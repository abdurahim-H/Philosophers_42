#include "philo.h"

// int check_philosopher_death(t_monitor_data *data, int i)
// {
//     if (!is_philosopher_alive(data->philo_data[i]))
//     {
//         long long current_time = get_current_time();
//         printf("%lld %d died\n", current_time, data->philo_data[i]->id);
//         printf("DEBUG: Death detected for philosopher %d\n", data->philo_data[i]->id);
//         handle_simulation_end_condition(data);
//         printf("DEBUG: Simulation end condition handled\n");
//         return (1);
//     }
//     return (0);
// }

int check_philosopher_death(t_monitor_data *data, int i)
{
    long long current_time;

    if (!is_philosopher_alive(data->philo_data[i]))
    {
        current_time = get_current_time();
        handle_simulation_end_condition(data);
        printf("%lld %d died\n", current_time, data->philo_data[i]->id);
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

int handle_simulation_end_condition(t_monitor_data *data)
{
    int result;

    result = safe_mutex_operation(&data->params->end_mutex, 1);
    if (result == 0)
    {
        data->params->simulation_end = 1;
        result = safe_mutex_operation(&data->params->end_mutex, 0);
    }
    if (result != 0)
    {
        printf("Error: Failed to handle simulation end condition\n");
        return (1);
    }
    return (0);
}

int	check_philosopher_status(t_monitor_data *data, int *philosophers_done)
{
	if (!check_all_philosophers_alive(data))
		return (0);
	if (count_philosophers_done_eating_loop(data, philosophers_done)
		== data->params->num_philosophers)
	{
		if (handle_simulation_end_condition(data) != 0)
			return (0);
		return (0);
	}
	return (1);
}

int     check_philosophers(t_monitor_data *data, int i)
{
    if (i >= data->params->num_philosophers)
        return (0);
    if (check_philosopher_death(data, i))
        return (1);
    return (check_philosophers(data, i + 1));
}

static int check_all_philosophers_finished(t_monitor_data *data)
{
    int philosophers_done;

    if (data->params->num_times_to_eat != -1 &&
        count_philosophers_done_eating_loop(data, &philosophers_done) 
        == data->params->num_philosophers)
    {
        handle_simulation_end_condition(data);
        return (1);
    }
    return (0);
}

void *monitor_philosophers(void *arg)
{
    t_monitor_data *data;

    data = (t_monitor_data *)arg;
    while (1)
    {
        if (check_philosophers(data, 0))
            return (NULL);
        if (check_all_philosophers_finished(data))
            return (NULL);
        usleep(100);
    }
}
