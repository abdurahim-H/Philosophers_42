#include "philo.h"

int	check_philosopher_death(t_monitor_data *data, int i)
{
	if (!is_philosopher_alive(data->philo_data[i]))
	{
		printf("Philosopher %d has died\n", data->philo_data[i]->id);
		handle_simulation_end_condition(data);
		return (1);
	}
	return (0);
}

int	safe_mutex_operation(pthread_mutex_t *mutex, int lock)
{
	int	result;

	if (lock)
		result = pthread_mutex_lock(mutex);
	else
		result = pthread_mutex_unlock(mutex);
	if (result != 0)
	{
		printf("Error: mutex operation failed\n");
		return (1);
	}
	return (0);
}

int	check_all_philosophers_alive(t_monitor_data *data)
{
	int	i;

	i = 0;
	while (i < data->params->num_philosophers)
	{
		if (!is_philosopher_alive(data->philo_data[i]))
			return (0);
		i++;
	}
	return (1);
}

int	count_philosophers_done_eating_loop(t_monitor_data *data,
	int *philosophers_done)
{
	int	i;
	int	meals_eaten;

	*philosophers_done = 0;
	i = 0;
	while (i < data->params->num_philosophers)
	{
		safe_mutex_operation(&data->philo_data[i]->data_mutex, 1);
		meals_eaten = data->philo_data[i]->philosopher.meals_eaten;
		safe_mutex_operation(&data->philo_data[i]->data_mutex, 0);
		if (data->params->num_times_to_eat != -1
			&& data->philo_data[i]->philosopher.meals_eaten
			>= data->params->num_times_to_eat)
		{
			(*philosophers_done)++;
		}
		i++;
	}
	return (*philosophers_done);
}

int	handle_simulation_end_condition(t_monitor_data *data)
{
	int	lock;
	int	result;

	lock = 1;
	result = safe_mutex_operation(&data->params->end_mutex, lock);
	if (result == 0)
	{
		data->params->simulation_end = 1;
		result = safe_mutex_operation(&data->params->end_mutex, !lock);
	}
	if (result != 0)
	{
		printf("Error: Failed to handle simulation end condition\n");
		return (1);
	}
	return (0);
}

int check_philosopher_status(t_monitor_data *data, int *philosophers_done)
{
    int i;
    int meals_eaten;

    *philosophers_done = 0;
    for (i = 0; i < data->params->num_philosophers; i++)
    {
        if (!data->philo_data[i]) // Check if the pointer is valid
            continue;

        pthread_mutex_lock(&data->philo_data[i]->data_mutex);
        meals_eaten = data->philo_data[i]->philosopher.meals_eaten;
        pthread_mutex_unlock(&data->philo_data[i]->data_mutex);

        if (!is_philosopher_alive(data->philo_data[i]))
        {
            pthread_mutex_lock(&data->params->end_mutex);
            if (!data->params->simulation_end)
            {
                printf("Philosopher %d has died\n", data->philo_data[i]->id);
                data->params->simulation_end = 1;
            }
            pthread_mutex_unlock(&data->params->end_mutex);
            return 0;
        }

        if (data->params->num_times_to_eat != -1 && meals_eaten >= data->params->num_times_to_eat)
            (*philosophers_done)++;
    }

    if (*philosophers_done == data->params->num_philosophers)
        return 0;
    return 1;
}

void *monitor_philosophers(void *arg)
{
    t_monitor_data *data;
    int philosophers_done;
    long long sleep_time;

    data = (t_monitor_data *)arg;
    sleep_time = (data->params->time_to_die / 10 > 1) ? data->params->time_to_die / 10 : 1;
    while (1)
    {
        usleep(sleep_time * 1000);
        
        pthread_mutex_lock(&data->params->end_mutex);
        if (data->params->simulation_end)
        {
            pthread_mutex_unlock(&data->params->end_mutex);
            break;
        }
        pthread_mutex_unlock(&data->params->end_mutex);

        if (!check_philosopher_status(data, &philosophers_done))
        {
            pthread_mutex_lock(&data->params->end_mutex);
            if (!data->params->simulation_end)
            {
                data->params->simulation_end = 1;
                if (philosophers_done == data->params->num_philosophers)
                    printf("All philosophers have eaten enough times\n");
                printf("Simulation ended\n");
            }
            pthread_mutex_unlock(&data->params->end_mutex);
            break;
        }
    }
    return NULL;
}