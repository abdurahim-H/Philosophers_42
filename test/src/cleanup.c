#include "philo.h"

void cleanup_mutexes(t_params *params)
{
    int i;

    i = 0;
    while (i < params->num_philosophers)
    {
        pthread_mutex_destroy(&(params->forks[i]));
        i++;
    }
    pthread_mutex_destroy(&params->end_mutex);
    pthread_mutex_destroy(&params->write_mutex);
    free(params->forks);
}

void cleanup_simulation(t_params *params, pthread_t *threads,
    t_philo_data **philo_data)
{
    cleanup_mutexes(params);
    free(threads);
    free(philo_data);
}

void cleanup_philosopher_mutexes(t_philo_data **philo_data,
    int num_philosophers)
{
    int i;

    i = 0;
    while (i < num_philosophers)
    {
        pthread_mutex_destroy(&philo_data[i]->data_mutex);
        i++;
    }
}