#include "philo.h"

void think(int philosopher_id)
{
    print_status(NULL, philosopher_id, "is thinking");
}

void eat(t_philo_data *philo, int left_fork, int right_fork)
{
    pthread_mutex_lock(&philo->params->forks[left_fork]);
    print_status(philo->params, philo->id, "has taken a fork");
    pthread_mutex_lock(&philo->params->forks[right_fork]);
    print_status(philo->params, philo->id, "has taken a fork");

    pthread_mutex_lock(&philo->data_mutex);
    philo->philosopher.last_meal_time = get_current_time();
    philo->philosopher.meals_eaten++;
    pthread_mutex_unlock(&philo->data_mutex);

    print_status(philo->params, philo->id, "is eating");
    ft_sleep(philo->params->time_to_eat, philo->params);

    pthread_mutex_unlock(&philo->params->forks[left_fork]);
    pthread_mutex_unlock(&philo->params->forks[right_fork]);
}

void ft_sleep(int sleep_time, t_params *params)
{
    long long start_time;
    long long current_time;

    start_time = get_current_time();
    while (!params->simulation_end)
    {
        current_time = get_current_time();
        if (current_time - start_time >= sleep_time)
            break;
        usleep(100); 
    }
}

void philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
{
    if (data->id % 2 == 0)
        usleep(1000);  // Prevent deadlock
    eat(data, left_fork, right_fork);
    print_status(data->params, data->id, "is sleeping");
    ft_sleep(data->params->time_to_sleep, data->params);
    print_status(data->params, data->id, "is thinking");
}

void *philo_lifecycle(void *arg)
{
    t_philo_data *philo;
    int left_fork;
    int right_fork;

    philo = (t_philo_data *)arg;
    left_fork = philo->id - 1;
    right_fork = philo->id % philo->params->num_philosophers;
    while (!philo->params->simulation_end)
    {
        philosopher_actions(philo, left_fork, right_fork);
        if (philo->params->simulation_end)
            break;
    }
    return (NULL);
}