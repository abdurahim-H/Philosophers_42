#include "philo.h"

int	create_philo_thread(pthread_t *thread, t_philo_data *data,
	void *(*philo_lifecycle)(void *))
{
	if (pthread_create(thread, NULL, philo_lifecycle, data) != 0)
	{
		free(data);
		return (1);
	}
	return (0);
}

int	create_philosophers(pthread_t *threads, int num_philosophers,
	void *(*philo_lifecycle)(void *), t_philo_data **philo_data)
{
	int	i;

	i = 0;
	while (i < num_philosophers)
	{
		if (create_philo_thread(&threads[i], philo_data[i],
				philo_lifecycle) != 0)
		{
			printf("Error: Unable to create thread\n");
			cleanup_exit(threads, philo_data, i);
		}
		i++;
	}
	return (0);
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
		free(philo_data[i]);
		i++;
	}
	free(philo_data);
	free(threads);
	exit(1);
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
	free(threads);
	free(philo_data);
}

int	main(int argc, char **argv)
{
	t_params		params;
	pthread_t		*threads;
	t_philo_data	**philo_data;

	if (parse_arguments(argc, argv, &params) != 0)
		return (1);
	if (allocate_threads(&threads, params.num_philosophers) != 0)
		return (1);
	philo_data = allocate_philo_data(params.num_philosophers, &params);
	if (!philo_data)
	{
		free(threads);
		return (1);
	}
	if (create_philosophers(threads, params.num_philosophers,
			philo_lifecycle, philo_data) != 0)
	{
		free(threads);
		free(philo_data);
		return (1);
	}
	initialize_mutexes(threads);
	join_and_free_threads(threads, philo_data, params.num_philosophers);
	return (0);
}
