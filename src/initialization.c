#include "philo.h"

int	initialize_mutexes(t_params *params)
{
	int		i;

	params->forks = malloc(params->num_philosophers * sizeof(pthread_mutex_t));
	if (params->forks == NULL)
	{
		printf("ERROR: MEMORY ALLOCATION FAILED FOR FORKS\n");
		return (-1);
	}
	i = 0;
	while (i < params->num_philosophers)
	{
		if (pthread_mutex_init(&(params->forks[i]), NULL) != 0)
		{
			printf("ERROR: MUTEX INITIALIZATION FAILED\n");
			while (--i >= 0)
				pthread_mutex_destroy(&(params->forks[i]));
			free(params->forks);
			return (-1);
		}
		i++;
	}
	return (0);
}

void	initialize_philosopher(t_philo_data *philo, int id, t_params *params)
{
	philo->id = id;
	philo->params = params;
	philo->philosopher.state = THINKING;
	philo->philosopher.last_meal_time = get_current_time();
	philo->philosopher.meals_eaten = 0;
	pthread_mutex_init(&philo->data_mutex, NULL);
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

int	initialize_philosophers(t_params *params, t_philo_data ***philo_data,
	pthread_t **threads)
{
	*philo_data = allocate_philo_data(params->num_philosophers, params);
	if (!*philo_data)
		return (-1);
	*threads = malloc(sizeof(pthread_t) * params->num_philosophers);
	if (!*threads)
	{
		cleanup_philosopher_mutexes(*philo_data, params->num_philosophers);
		free(*philo_data);
		return (-1);
	}
	return (0);
}
