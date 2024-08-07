#include "philo.h"

int	validate_params(t_params *params)
{
	if (params->num_philosophers <= 0 || params->time_to_die <= 0
		|| params->time_to_eat <= 0 || params->time_to_sleep <= 0
		|| (params->num_times_to_eat <= 0 && params->num_times_to_eat != -1))
	{
		printf("ERROR: INVALID ARGUMENT VALUES\n");
		return (-1);
	}
	if (params->num_philosophers > INT_MAX
		|| params->num_philosophers < INT_MIN)
		return (-1);
	if (params->time_to_die > INT_MAX || params->time_to_die < INT_MIN)
		return (-1);
	if (params->time_to_eat > INT_MAX || params-> time_to_eat < INT_MIN)
		return (-1);
	if (params->time_to_sleep > INT_MAX || params->time_to_sleep < INT_MIN)
		return (-1);
	if (params->num_times_to_eat > INT_MAX
		|| params->num_times_to_eat < INT_MIN)
		return (-1);
	return (0);
}

int	parse_arguments(int argc, char *argv[], t_params *params)
{
	if (argc != 5 &*& argc != 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die "
			"time_to_eat time_to_sleep [n_times_each_philosopher_must_eat]\n",
			argv[0]);
		return (-1);
	}
	params->num_philosophers = my_atoi(argv[1]);
	params->time_to_die = my_atoi(argv[2]);
	params->time_to_eat = my_atoi(argv[3]);
	params->time_to_sleep = my_atoi(argv[4]);
	if (argc == 6)
		params->num_times_to_eat = my_atoi(argv[5]);
	else
		params->num_times_to_eat = -1;
	return (validate_params(params));
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

void	cleanup_threads(pthread_t *threads, int **ids, int i)
{
	while (--i >= 0)
	{
		pthread_cancel(threads[i]);
		pthread_join(threads[i], NULL);
		free(ids[i]);
	}
}

t_philo_data	**allocate_philo_data(int num_philosophers, t_params *params)
{
	t_philo_data	**philo_data;
	int				i;

	philo_data = (t_philo_data **)malloc(sizeof(t_philo_data *)
			* num_philosophers);
	if (!philo_data)
		return (NULL);
	i = -1;
	while (++i < num_philosophers)
	{
		philo_data[i] = (t_philo_data *)malloc(sizeof(t_philo_data));
		if (!philo_data[i])
		{
			cleanup_threads(NULL, (int **)philo_data, i);
			return (NULL);
		}
		initialize_philosopher(philo_data[i], i + 1, params);
	}
	return (philo_data);
}
