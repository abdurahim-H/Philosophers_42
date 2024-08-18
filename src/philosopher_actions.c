#include "philo.h"

void    think(t_philo_data *philo)
{
    long long current_time = get_current_time();
    printf("%lld %d is thinking\n", current_time, philo->id);
}

void    take_fork(t_philo_data *philo)
{
    long long current_time = get_current_time();
    printf("%lld %d has taken a fork\n", current_time, philo->id);
}

// void    eat(t_philo_data *philo, int left_fork, int right_fork)
// {
//     int                 first_fork;
//     int                 second_fork;
//     struct timeval      tv;
//     long long           current_time;
//     long long           start_time;

//     first_fork = left_fork;
//     second_fork = right_fork;
//     if (philo->id % 2 == 0)
//     {
//         first_fork = right_fork;
//         second_fork = left_fork;
//     }
//     pthread_mutex_lock(&(philo->params->forks[first_fork]));
//     pthread_mutex_lock(&(philo->params->forks[second_fork]));
//     gettimeofday(&tv, NULL);
//     current_time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
//     safe_mutex_operation(&philo->data_mutex, 1);
//     philo->philosopher.state = EATING;
//     philo->philosopher.last_meal_time = current_time;
//     philo->philosopher.meals_eaten += 1;
//     safe_mutex_operation(&philo->data_mutex, 0);
//     printf("%lld Philosopher %d is eating\n", current_time, philo->id);
//     start_time = get_current_time();
//     while (get_current_time() - start_time < philo->params->time_to_eat)
//     {
//         usleep(100);
//     }
//     pthread_mutex_unlock(&(philo->params->forks[second_fork]));
//     pthread_mutex_unlock(&(philo->params->forks[first_fork]));
// }

void    eat(t_philo_data *philo, int left_fork, int right_fork)
{
    int                 first_fork;
    int                 second_fork;
    long long           current_time;
    long long           start_time;

    first_fork = left_fork;
    second_fork = right_fork;
    if (philo->id % 2 == 0)
    {
        first_fork = right_fork;
        second_fork = left_fork;
    }
    pthread_mutex_lock(&(philo->params->forks[first_fork]));
    take_fork(philo);
    pthread_mutex_lock(&(philo->params->forks[second_fork]));
    take_fork(philo);
    current_time = get_current_time();
    safe_mutex_operation(&philo->data_mutex, 1);
    philo->philosopher.state = EATING;
    philo->philosopher.last_meal_time = current_time;
    philo->philosopher.meals_eaten += 1;
    safe_mutex_operation(&philo->data_mutex, 0);
    printf("%lld %d is eating\n", current_time, philo->id);
    start_time = current_time;
    while (get_current_time() - start_time < philo->params->time_to_eat)
    {
        usleep(100);
    }
    pthread_mutex_unlock(&(philo->params->forks[second_fork]));
    pthread_mutex_unlock(&(philo->params->forks[first_fork]));
}

// void    ft_sleep(int philosopher_id, t_params *params)
// {
//     long long start_time;

//     printf("Philosopher %d is sleeping\n", philosopher_id);
//     start_time = get_current_time();
//     while (get_current_time() - start_time < params->time_to_sleep)
//     {
//         usleep(100);
//     }
// }

void    ft_sleep(t_philo_data *philo)
{
    long long start_time;
    long long current_time;

    current_time = get_current_time();
    printf("%lld %d is sleeping\n", current_time, philo->id);
    start_time = current_time;
    while (get_current_time() - start_time < philo->params->time_to_sleep)
    {
        usleep(100);
    }
}


bool    is_philosopher_alive(t_philo_data *philo)
{
    long long           current_time;
    long long           time_since_last_meal;
    long long           last_meal_time;

    current_time = get_current_time();
    if (safe_mutex_operation(&philo->data_mutex, 1) != 0)
        return (false);
    last_meal_time = philo->philosopher.last_meal_time;
    safe_mutex_operation(&philo->data_mutex, 0);
    time_since_last_meal = current_time - last_meal_time;
    return (time_since_last_meal <= philo->params->time_to_die);
}

void    *philo_lifecycle(void *arg)
{
    t_philo_data    *data;
    int             left_fork;
    int             right_fork;
    int             should_continue;

    data = (t_philo_data *)arg;
    right_fork = data->id - 1;
    left_fork = (data->id % data->params->num_philosophers);
    should_continue = 1;
    while (should_continue)
    {
        pthread_mutex_lock(&data->params->end_mutex);
        should_continue = !data->params->simulation_end;
        pthread_mutex_unlock(&data->params->end_mutex);
        if (should_continue)
            philosopher_actions(data, left_fork, right_fork);
        usleep(100);
    }
    return (NULL);
}
