#include "philo.h"

bool is_philosopher_alive(t_philo_data *philo)
{
    long long current_time;
    long long last_meal_time;
    bool is_alive;

    current_time = get_current_time();
    pthread_mutex_lock(&philo->data_mutex);
    last_meal_time = philo->philosopher.last_meal_time;
    is_alive = (current_time - last_meal_time <= philo->params->time_to_die);
    pthread_mutex_unlock(&philo->data_mutex);
    return is_alive;
}

int check_philosopher_death(t_monitor_data *data, int i)
{
    if (!is_philosopher_alive(data->philo_data[i]))
    {
        print_status(data->params, data->philo_data[i]->id, "died");
        safe_mutex_operation(&data->params->end_mutex, 1);
        data->params->simulation_end = 1;
        safe_mutex_operation(&data->params->end_mutex, 0);
        return (1);
    }
    return (0);
}


int check_philosopher_status(t_monitor_data *data, int *philosophers_done)
{
    int i;

    i = 0;
    *philosophers_done = 0;
    while (i < data->params->num_philosophers)
    {
        if (!is_philosopher_alive(data->philo_data[i]))
        {
            pthread_mutex_lock(&data->params->end_mutex);
            if (!data->params->simulation_end)
            {
                data->params->simulation_end = 1;
                print_status(data->params, data->philo_data[i]->id, "died");
            }
            pthread_mutex_unlock(&data->params->end_mutex);
            return (0);
        }
        pthread_mutex_lock(&data->philo_data[i]->data_mutex);
        if (data->params->num_times_to_eat != -1 &&
            data->philo_data[i]->philosopher.meals_eaten >= data->params->num_times_to_eat)
        {
            (*philosophers_done)++;
        }
        pthread_mutex_unlock(&data->philo_data[i]->data_mutex);
        i++;
    }
    return (1);
}

int handle_simulation_end_condition(t_monitor_data *data)
{
    safe_mutex_operation(&data->params->end_mutex, 1);
    data->params->simulation_end = 1;
    safe_mutex_operation(&data->params->end_mutex, 0);
    return (0);
}

void *monitor_philosophers(void *arg)
{
    t_monitor_data *data;
    int philosophers_done;

    data = (t_monitor_data *)arg;
    while (!data->params->simulation_end)
    {
        if (!check_philosopher_status(data, &philosophers_done))
            break;
        if (philosophers_done == data->params->num_philosophers)
            break;
        usleep(1000);
    }
    return (NULL);
}