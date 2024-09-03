#include "philo.h"

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

void	release_forks(t_philo_data *data, int left_fork, int right_fork)
{
	pthread_mutex_unlock(&(data->params->forks[right_fork]));
	pthread_mutex_unlock(&(data->params->forks[left_fork]));
}
