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
