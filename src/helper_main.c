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

// t_philo_data	**allocate_philo_data(int num_philosophers, t_params *params)
// {
// 	t_philo_data	**philo_data;
// 	int				i;

// 	philo_data = (t_philo_data **)malloc(sizeof(t_philo_data *)
// 			* num_philosophers);
// 	if (!philo_data)
// 		return (NULL);
// 	i = -1;
// 	while (++i < num_philosophers)
// 	{
// 		philo_data[i] = (t_philo_data *)malloc(sizeof(t_philo_data));
// 		if (!philo_data[i])
// 		{
// 			cleanup_threads(NULL, (int **)philo_data, i);
// 			return (NULL);
// 		}
// 		philo_data[i]->id = i + 1;
// 		philo_data[i]->params = params;
// 	}
// 	return (philo_data);
// }
