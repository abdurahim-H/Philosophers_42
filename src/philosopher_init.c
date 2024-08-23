#include "philo.h"


void initialize_philosopher(t_philo_data *philo, int id, t_params *params)
{
    philo->id = id;
    philo->params = params;
    philo->philosopher.state = THINKING;
    philo->philosopher.last_meal_time = get_current_time();
    philo->philosopher.meals_eaten = 0;
    pthread_mutex_init(&philo->data_mutex, NULL);
}

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
{
    if (check_simulation_end(data->params) || !is_philosopher_alive(data))
        return;

    think(data);
    
    if (check_simulation_end(data->params) || !is_philosopher_alive(data))
        return;

    if (take_forks(data, left_fork, right_fork) == 0)
    {
        if (!check_simulation_end(data->params) && is_philosopher_alive(data))
        {
            eat(data);
        }
        release_forks(data, left_fork, right_fork);
    }
    
    if (check_simulation_end(data->params) || !is_philosopher_alive(data))
        return;

    ft_sleep(data);
}
