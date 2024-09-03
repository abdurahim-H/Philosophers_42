#include "philo.h"

void	*philo_lifecycle(void *arg)
{
	t_philo_data		*data;
	int					left_fork;
	int					right_fork;

	data = (t_philo_data *)arg;
	right_fork = data->id - 1;
	left_fork = (data->id % data->params->num_philosophers);
	while (!check_simulation_end(data->params))
	{
		philosopher_actions(data, left_fork, right_fork);
	}
	return (NULL);
}

void	join_and_free_threads(pthread_t *threads, t_philo_data **philo_data,
		int num_philosophers)
{
	int	i;

	i = -1;
	while (++i < num_philosophers)
	{
		pthread_join(threads[i], NULL);
		free(philo_data[i]);
	}
	// free(threads);
	// free(philo_data);
	safe_free((void **)&threads);
	safe_free((void **)&philo_data);
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
