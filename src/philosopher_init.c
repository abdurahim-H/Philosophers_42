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

// void	philosopher_actions_else(t_philo_data *data,
// 	int left_fork, int right_fork)
// {
// 	think(data->id);
// 	if (!is_philosopher_alive(data))
// 		return ;
// 	eat(data, left_fork, right_fork);
// 	if (!is_philosopher_alive(data))
// 		return ;
// 	ft_sleep(data->id, data->params);
// }

// void	philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
// {
// 	long long	time_to_death;
// 	long long	last_meal_time;

// 	if (!is_philosopher_alive(data))
// 		return ;
// 	safe_mutex_operation(&data->data_mutex, 1);
// 	last_meal_time = data->philosopher.last_meal_time;
// 	safe_mutex_operation(&data->data_mutex, 0);
// 	time_to_death = data->params->time_to_die - (get_current_time()
// 			- last_meal_time);
// 	if (time_to_death < data->params->time_to_die)
// 		eat(data, left_fork, right_fork);
// 	else
// 		philosopher_actions_else(data, left_fork, right_fork);
// }

bool philosopher_actions_else(t_philo_data *data, int left_fork, int right_fork)
{
    think(data->id);
    if (!is_philosopher_alive(data))
        return false;
    if (!eat(data, left_fork, right_fork))
        return false;
    if (!is_philosopher_alive(data))
        return false;
    ft_sleep(data->id, data->params);
    return true;
}

bool philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
{
    long long   time_to_death;
    long long   last_meal_time;

    pthread_mutex_lock(&data->params->end_mutex);
    if (data->params->simulation_end)
    {
        pthread_mutex_unlock(&data->params->end_mutex);
        return false;
    }
    pthread_mutex_unlock(&data->params->end_mutex);

    if (!is_philosopher_alive(data))
        return false;
    safe_mutex_operation(&data->data_mutex, 1);
    last_meal_time = data->philosopher.last_meal_time;
    safe_mutex_operation(&data->data_mutex, 0);
    time_to_death = data->params->time_to_die - (get_current_time()
            - last_meal_time);
    if (time_to_death < data->params->time_to_eat)
        return eat(data, left_fork, right_fork);
    else
        return philosopher_actions_else(data, left_fork, right_fork);
}