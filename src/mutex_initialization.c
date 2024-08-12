#include "philo.h"

int initialize_mutexes(t_params *params)
{
    int i;

    params->forks = malloc(params->num_philosophers * sizeof(pthread_mutex_t));
    if (!params->forks)
        return 1;

    for (i = 0; i < params->num_philosophers; i++)
    {
        if (pthread_mutex_init(&params->forks[i], NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&params->forks[i]);
            free(params->forks);
            return 1;
        }
    }

    if (pthread_mutex_init(&params->end_mutex, NULL) != 0)
    {
        for (i = 0; i < params->num_philosophers; i++)
            pthread_mutex_destroy(&params->forks[i]);
        free(params->forks);
        return 1;
    }

    return 0;
}