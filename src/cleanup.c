#include "philo.h"

void	cleanup_simulation(t_params *params, pthread_t *threads,
	t_philo_data **philo_data)
{
	if (params && threads && philo_data)
	{
		cleanup_philosopher_mutexes(philo_data, params->num_philosophers);
		free(threads);
		free(philo_data);
		cleanup_mutexes(params);
	}
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

void	cleanup_threads(pthread_t *threads, int **ids, int i)
{
	while (--i >= 0)
	{
		pthread_cancel(threads[i]);
		pthread_join(threads[i], NULL);
		free(ids[i]);
	}
}

void	cleanup_exit(pthread_t *threads, t_philo_data **philo_data,
		int num_created)
{
	int	i;

	i = 0;
	while (i < num_created)
	{
		pthread_cancel(threads[i]);
		pthread_join(threads[i], NULL);
		pthread_mutex_destroy(&(philo_data[i]->data_mutex));
		free(philo_data[i]);
		i++;
	}
	free(philo_data);
	free(threads);
	exit(1);
}
