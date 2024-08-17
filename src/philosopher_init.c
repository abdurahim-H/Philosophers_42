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

// void	philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
// {
// 	long long	time_to_death;
// 	t_state		current_state;
// 	long long	last_meal_time;

// 	safe_mutex_operation(&data->data_mutex, 1);
// 	current_state = data->philosopher.state;
// 	last_meal_time = data->philosopher.last_meal_time;
// 	safe_mutex_operation(&data->data_mutex, 0);
// 	time_to_death = data->params->time_to_die - (get_current_time()
// 			- last_meal_time);
// 	if (current_state != EATING && time_to_death
// 		< data->params->time_to_eat)
// 	{
// 		eat(data, left_fork, right_fork);
// 	}
// 	else
// 	{
// 		think(data->id);
// 		eat(data, left_fork, right_fork);
// 		ft_sleep(data->id, data->params);
// 	}
// }


void    philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
{
    int current_state;

    safe_mutex_operation(&data->data_mutex, 1);
    current_state = data->philosopher.state;
    safe_mutex_operation(&data->data_mutex, 0);
    if (current_state != EATING)
    {
        think(data->id);
        eat(data, left_fork, right_fork);
        ft_sleep(data->id, data->params);
    }
    else
    {
        eat(data, left_fork, right_fork);
    }
}