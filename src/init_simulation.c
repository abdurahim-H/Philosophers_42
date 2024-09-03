#include "philo.h"

int	initialize_simulation(t_params *params, pthread_t **threads,
	t_philo_data ***philo_data)
{
	if (init_end_mutex(params) != 0)
		return (1);
	if (init_forks_and_threads(params, threads) != 0)
		return (1);
	if (init_philo_data(params, threads, philo_data) != 0)
		return (1);
	return (0);
}

int	init_end_mutex(t_params *params)
{
	params->simulation_end = 0;
	if (pthread_mutex_init(&params->end_mutex, NULL) != 0)
	{
		printf("Failed to initialize end_mutex\n");
		return (1);
	}
	return (0);
}

int	init_forks_and_threads(t_params *params, pthread_t **threads)
{
	if (initialize_mutexes(params) != 0)
	{
		// pthread_mutex_destroy(&params->end_mutex);
		safe_mutex_destroy(&params->end_mutex);
		return (1);
	}
	if (allocate_threads(threads, params->num_philosophers) != 0)
	{
		pthread_mutex_destroy(&params->end_mutex);
		safe_mutex_destroy(&params->end_mutex);
		cleanup_mutexes(params);
		return (1);
	}
	return (0);
}

int	init_philo_data(t_params *params, pthread_t **threads,
	t_philo_data ***philo_data)
{
	*philo_data = allocate_philo_data(params->num_philosophers, params);
	if (!*philo_data)
	{
		pthread_mutex_destroy(&params->end_mutex);
		free(*threads);
		cleanup_mutexes(params);
		return (1);
	}
	return (0);
}

int	allocate_threads(pthread_t **threads, int num_philosophers)
{
	*threads = (pthread_t *)malloc(sizeof(pthread_t) * num_philosophers);
	if (!*threads)
	{
		printf("ERROR: UNABLE TO ALLOCATE MEMORY FOR THREADS\n");
		return (1);
	}
	return (0);
}
