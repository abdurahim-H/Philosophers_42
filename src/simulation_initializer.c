#include "philo.h"

int	initialize_simulation(t_params *params, pthread_t **threads,
	t_philo_data ***philo_data)
{
	params->simulation_end = 0;
	if (pthread_mutex_init(&params->end_mutex, NULL) != 0)
	{
		return (printf("Failed to initialize end_mutex\n"), 1);
	}
	if (initialize_mutexes(params) != 0)
	{
		pthread_mutex_destroy(&params->end_mutex);
		return (1);
	}
	if (allocate_threads(threads, params->num_philosophers) != 0)
	{
		pthread_mutex_destroy(&params->end_mutex);
		cleanup_mutexes(params);
		return (1);
	}
	*philo_data = allocate_philo_data(params->num_philosophers, params);
	if (!philo_data)
	{
		pthread_mutex_destroy(&params->end_mutex);
		free(*threads);
		cleanup_mutexes(params);
		return (1);
	}
	return (0);
}

void	cleanup_simulation(t_params *params, pthread_t *threads,
	t_philo_data **philo_data)
{
	cleanup_philosopher_mutexes(philo_data, params->num_philosophers);
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

void	cleanup_philosopher_mutexes(t_philo_data **philo_data,
	int num_philosophers)
{
	int	i;

	i = 0;
	while (i < num_philosophers)
	{
		pthread_mutex_destroy(&(philo_data[i]->data_mutex));
		i++;
	}
}
