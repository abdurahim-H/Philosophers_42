#include "philo.h"

void	think(int philosopher_id)
{
	printf("Philosopher %d is thinking\n", philosopher_id);
	usleep((rand() % 1000 + 1) * 1000);
}

void	eat(int philosopher_id, t_params *params, int left_fork, int right_fork)
{
	pthread_mutex_lock(&(params->forks[right_fork]));
	pthread_mutex_lock(&(params->forks[left_fork]));
	printf("Philosopher %d is eating\n", philosopher_id);
	usleep(params->time_to_eat * 1000);
	pthread_mutex_unlock(&(params->forks[right_fork]));
	pthread_mutex_unlock(&(params->forks[left_fork]));
}

void	sleep(int philosopher_id, t_params *params)
{
	printf("Philosopher %d is sleeping\n", philosopher_id);
	usleep(params->time_to_sleep * 1000);
}

void	*philo_lifecycle(void *arg)
{
	t_philo_data	*data;
	t_params		*params;
	int				id;
	int				left_fork;
	int				right_fork;

	data = (t_philo_data *)arg;
	id = data->id;
	params = data->params;
	right_fork = id;
	if (id == 0)
		left_fork = params->num_philosophers - 1;
	else
		left_fork = id - 1;
	while (1)
	{
		think(id);
		eat(id, params, left_fork, right_fork);
		sleep(id, params);
	}
	return (NULL);
}
