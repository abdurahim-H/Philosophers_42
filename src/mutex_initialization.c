#include "philo.h"

int	initialize_mutexes(t_params *params)
{
	int		i;
	params->forks = malloc(params->num_philosophers * sizeof(pthread_mutex_t));
}