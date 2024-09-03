#include "philo.h"

void	*monitor_philosophers(void *arg)
{
	t_monitor_data	*data;

	data = (t_monitor_data *)arg;
	while (1)
	{
		if (check_philosophers(data))
			return (NULL);
		if (check_all_philosophers_finished(data))
			return (NULL);
		usleep(100);
	}
}

int	check_philosophers(t_monitor_data *data)
{
	int i;

	i = 0;
	while (i < data->params->num_philosophers)
	{
		if (check_philosopher_death(data, i))
			return (1);
		i++;
	}
	return (0);
}

int	check_philosopher_death(t_monitor_data *data, int i)
{
	long long	current_time;

	if (!is_philosopher_alive(data->philo_data[i]))
	{
		current_time = get_current_time();
		handle_simulation_end_condition(data);
		printf("%lld %d died\n", current_time, data->philo_data[i]->id);
		return (1);
	}
	return (0);
}

int	check_all_philosophers_finished(t_monitor_data *data)
{
	int	philosophers_done;

	if (data->params->num_times_to_eat != -1
		&& count_philosophers_done_eating_loop(data, &philosophers_done)
		== data->params->num_philosophers)
	{
		handle_simulation_end_condition(data);
		return (1);
	}
	return (0);
}

int	handle_simulation_end_condition(t_monitor_data *data)
{
	int	result;

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
