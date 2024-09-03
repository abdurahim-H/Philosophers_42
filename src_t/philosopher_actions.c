#include "philo.h"

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

bool	is_philosopher_alive(t_philo_data *philo)
{
	long long	current_time;
	long long	time_since_last_meal;
	long long	last_meal_time;

	current_time = get_current_time();
	if (safe_mutex_operation(&philo->data_mutex, 1) != 0)
		return (false);
	last_meal_time = philo->philosopher.last_meal_time;
	safe_mutex_operation(&philo->data_mutex, 0);
	time_since_last_meal = current_time - last_meal_time;
	return (time_since_last_meal < philo->params->time_to_die);
}

int	check_simulation_end(t_params *params)
{
	int	end;

	end = 0;
	if (safe_mutex_operation(&params->end_mutex, 1) == 0)
	{
		end = params->simulation_end;
		safe_mutex_operation(&params->end_mutex, 0);
	}
	return (end);
}

int	handle_single_philosopher(t_philo_data *data, int fork)
{
	if (pthread_mutex_lock(&(data->params->forks[fork])) != 0)
		return (1);
	printf("%lld %d has taken a fork\n", get_current_time(), data->id);
	while (!check_simulation_end(data->params))
	{
		if (!is_philosopher_alive(data))
		{
			pthread_mutex_unlock(&(data->params->forks[fork]));
			return (1);
		}
		usleep(100);
	}
	pthread_mutex_unlock(&(data->params->forks[fork]));
	return (1);
}

int	take_first_fork(t_philo_data *data, int fork)
{
	if (pthread_mutex_lock(&(data->params->forks[fork])) != 0)
		return (1);
	if (check_simulation_end(data->params))
	{
		pthread_mutex_unlock(&(data->params->forks[fork]));
		return (1);
	}
	printf("%lld %d has taken a fork\n", get_current_time(), data->id);
	return (0);
}

int	take_second_fork(t_philo_data *data, int first, int second)
{
	if (pthread_mutex_lock(&(data->params->forks[second])) != 0)
	{
		pthread_mutex_unlock(&(data->params->forks[first]));
		return (1);
	}
	if (check_simulation_end(data->params))
	{
		pthread_mutex_unlock(&(data->params->forks[second]));
		pthread_mutex_unlock(&(data->params->forks[first]));
		return (1);
	}
	printf("%lld %d has taken a fork\n", get_current_time(), data->id);
	return (0);
}

int	take_forks(t_philo_data *data, int left_fork, int right_fork)
{
	int	first_fork;
	int	second_fork;

	if (data->params->num_philosophers == 1)
		return (handle_single_philosopher(data, left_fork));
	if (data->id % 2 == 0)
	{
		first_fork = right_fork;
		second_fork = left_fork;
	}
	else
	{
		first_fork = left_fork;
		second_fork = right_fork;
	}
	if (take_first_fork(data, first_fork) != 0)
		return (1);
	if (take_second_fork(data, first_fork, second_fork) != 0)
		return (1);
	return (0);
}

void	release_forks(t_philo_data *data, int left_fork, int right_fork)
{
	pthread_mutex_unlock(&(data->params->forks[right_fork]));
	pthread_mutex_unlock(&(data->params->forks[left_fork]));
}

void	*philo_lifecycle(void *arg)
{
	t_philo_data		*data;
	int					left_fork;
	int					right_fork;

	data = (t_philo_data *)arg;
	right_fork = data->id - 1;
	left_fork = (data->id % data->params->num_philosophers);
	while (!check_simulation_end(data->params))
	{
		philosopher_actions(data, left_fork, right_fork);
	}
	return (NULL);
}
