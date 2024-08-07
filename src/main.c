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
		pthread_mutex_destroy(&(philo_data[i]->data_mutex));
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
	t_sim_data			sim_data;
	t_params			params;

	srand(time(NULL));
	sim_data.params = &params;
	if (initialize_all(argc, argv, &sim_data) != 0)
		return (1);
	if (create_all_threads(&sim_data) != 0)
	{
		cleanup_simulation(sim_data.params, sim_data.threads,
			sim_data.philo_data);
		return (1);
	}
	join_and_cleanup(&sim_data);
	return (0);
}
