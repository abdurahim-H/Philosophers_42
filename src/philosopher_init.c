#include "philo.h"

void	initialize_philosopher(t_philo_data *philo, int id, t_params *params)
{
	struct timeval		tv;
	long long			current_time;

	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000LL) + (tv.tv_sec / 1000);
	philo->id = id;
	philo->params = params;
	philo->philosopher.state = THINKING;
	philo->philosopher.last_meal_time = current_time;
	philo->philosopher.meals_eaten = 0;
	pthread_mutex_init(&philo->data_mutex, NULL);
}

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

// void    philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
// {
//     safe_mutex_operation(&data->data_mutex, 1);
//     data->philosopher.state = THINKING;
//     safe_mutex_operation(&data->data_mutex, 0);
//     think(data);
    
//     eat(data, left_fork, right_fork);
    
//     safe_mutex_operation(&data->data_mutex, 1);
//     data->philosopher.state = SLEPING;
//     safe_mutex_operation(&data->data_mutex, 0);
//     ft_sleep(data);
// }

void philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
{
    if (check_simulation_end(data->params))
        return;

    // Thinking
    safe_mutex_operation(&data->data_mutex, 1);
    data->philosopher.state = THINKING;
    safe_mutex_operation(&data->data_mutex, 0);
    think(data);
    
    if (check_simulation_end(data->params))
        return;

    // Eating
    if (take_forks(data, left_fork, right_fork) == 0)
    {
        if (!check_simulation_end(data->params))
        {
            eat(data);
        }
        release_forks(data, left_fork, right_fork);
    }
    
    if (check_simulation_end(data->params))
        return;

    // Sleeping
    safe_mutex_operation(&data->data_mutex, 1);
    data->philosopher.state = SLEPING;
    safe_mutex_operation(&data->data_mutex, 0);
    ft_sleep(data);
}