#include "philo.h"

int initialize_mutexes(t_params *params)
{
    int i;

    params->forks = malloc(params->num_philosophers * sizeof(pthread_mutex_t));
    if (!params->forks)
        return (1);
    i = 0;
    while (i < params->num_philosophers)
    {
        if (pthread_mutex_init(&(params->forks[i]), NULL) != 0)
            return (1);
        i++;
    }
    if (pthread_mutex_init(&params->end_mutex, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&params->write_mutex, NULL) != 0)
        return (1);
    return (0);
}

void initialize_philosopher(t_philo_data *philo, int id, t_params *params)
{
    philo->id = id;
    philo->params = params;
    philo->philosopher.id = id;
    philo->philosopher.state = THINKING;
    philo->philosopher.last_meal_time = get_current_time();
    philo->philosopher.meals_eaten = 0;
    pthread_mutex_init(&philo->data_mutex, NULL);
}

int initialize_simulation(t_params *params, pthread_t **threads,
                          t_philo_data ***philo_data)
{
    int i;

    params->simulation_end = 0;
    if (initialize_mutexes(params) != 0)
        return (1);
    *threads = malloc(params->num_philosophers * sizeof(pthread_t));
    if (!*threads)
        return (1);
    *philo_data = malloc(params->num_philosophers * sizeof(t_philo_data *));
    if (!*philo_data)
        return (1);
    i = 0;
    while (i < params->num_philosophers)
    {
        (*philo_data)[i] = malloc(sizeof(t_philo_data));
        if (!(*philo_data)[i])
            return (1);
        initialize_philosopher((*philo_data)[i], i + 1, params);
        i++;
    }
    return (0);
}