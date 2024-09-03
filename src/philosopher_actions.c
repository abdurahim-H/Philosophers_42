#include "philo.h"

void	philosopher_actions(t_philo_data *data, int left_fork, int right_fork)
{
	if (check_simulation_end(data->params) || !is_philosopher_alive(data))
		return ;
	think(data);
	if (check_simulation_end(data->params) || !is_philosopher_alive(data))
		return ;
	if (take_forks(data, left_fork, right_fork) == 0)
	{
		if (!check_simulation_end(data->params) && is_philosopher_alive(data))
		{
			eat(data);
		}
		release_forks(data, left_fork, right_fork);
	}
	if (check_simulation_end(data->params) || !is_philosopher_alive(data))
		return ;
	ft_sleep(data);
}

void	think(t_philo_data *philo)
{
	long long	current_time;

	current_time = get_current_time();
	printf("%lld %d is thinking\n", current_time, philo->id);
	usleep(1000);
}

void	take_fork(t_philo_data *philo)
{
	long long	current_time;

	current_time = get_current_time();
	printf("%lld %d has taken a fork\n", current_time, philo->id);
}

void	eat(t_philo_data *philo)
{
	long long	current_time;
	long long	start_time;

	if (check_simulation_end(philo->params))
		return ;
	current_time = get_current_time();
	safe_mutex_operation(&philo->data_mutex, 1);
	philo->philosopher.state = EATING;
	philo->philosopher.last_meal_time = current_time;
	philo->philosopher.meals_eaten += 1;
	safe_mutex_operation(&philo->data_mutex, 0);
	if (!check_simulation_end(philo->params))
		printf("%lld %d is eating\n", current_time, philo->id);
	start_time = current_time;
	while (get_current_time() - start_time < philo->params->time_to_eat)
	{
		if (check_simulation_end(philo->params))
			return ;
		usleep(1000);
	}
}

void	ft_sleep(t_philo_data *philo)
{
	long long	start_time;
	long long	current_time;

	current_time = get_current_time();
	printf("%lld %d is sleeping\n", current_time, philo->id);
	start_time = current_time;
	while (get_current_time() - start_time < philo->params->time_to_sleep)
	{
		if (check_simulation_end(philo->params))
			return ;
		usleep(1000);
	}
}
