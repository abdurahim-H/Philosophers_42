#include "philo.h"

#define MAX_EXECUTIONS 200
#define COUNTER_FILE ".philo_counter"

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

int	initialize_all(int argc, char **argv, t_sim_data *sim_data)
{
	if (parse_arguments(argc, argv, sim_data->params) != 0)
		return (1);
	if (initialize_simulation(sim_data->params,
			&sim_data->threads, &sim_data->philo_data) != 0)
	{
		return (1);
	}
	return (0);
}

int	create_all_threads(t_sim_data *sim_data)
{
	t_monitor_data			monitor_data;

	monitor_data.params = sim_data->params;
	monitor_data.philo_data = sim_data->philo_data;
	if (pthread_create(&sim_data->monitor_thread, NULL, monitor_philosophers,
			&monitor_data) != 0)
	{
		printf("Failed to create monitor thread\n");
		return (1);
	}
	if (create_philosophers(sim_data->threads,
			sim_data->params->num_philosophers, philo_lifecycle,
			sim_data->philo_data) != 0)
	{
		pthread_cancel(sim_data->monitor_thread);
		return (1);
	}
	return (0);
}

void	join_and_cleanup(t_sim_data *sim_data)
{
	pthread_join(sim_data->monitor_thread, NULL);
	join_and_free_threads(sim_data->threads, sim_data->philo_data,
		sim_data->params->num_philosophers);
	cleanup_mutexes(sim_data->params);
}

void	safe_mutex_destroy(pthread_mutex_t *mutex)
{
	if (mutex)
	{
		pthread_mutex_destroy(mutex);
	}
}
