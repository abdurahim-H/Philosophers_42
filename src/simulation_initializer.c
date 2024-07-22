#include "philo.h"

int	initialize_simulation(t_params *params, pthread_t **threads,
	t_philo_data ***philo_data)
{
	if (initialize_mutexes(params) != 0)
		return (1);
	if (allocate_threads(threads, params->num_philosophers) != 0)
	{
		cleanup_mutexes(params);
		return (1);
	}
	*philo_data = allocate_philo_data(params->num_philosophers, params);
	if (!philo_data)
	{
		free(*threads);
		cleanup_mutexes(params);
		return (1);
	}
	return (0);
}

void	cleanup_simulation(t_params *params, pthread_t *threads,
	t_philo_data **philo_data)
{
	free(threads);
	free(philo_data);
	cleanup_mutexes(params);
}

void	cleanup_mutexes(t_params *params)
{
	int	i;

	i = 0;
	while (i < params->num_philosophers)
	{
		pthread_mutex_destroy(&(params->forks[i]));
		i++;
	}
	free(params->forks);
}
