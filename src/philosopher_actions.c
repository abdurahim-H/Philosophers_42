#include "philo.h"

void	think(int philosopher_id)
{
	printf("Philosopher %d is thinking\n", philosopher_id);
}

void	eat(int philosopher_id)
{
	printf("Philosopher %d is eating\n", philosopher_id);
}

void	sleep(int philosopher_id)
{
	printf("Philosopher %d is sleeping\n", philosopher_id);
}

void	*philo_lifecycle(void *arg)
{
	t_philo_data	*data;
	t_params		*params;
	int				id;

	data = (t_philo_data *)arg;
	id = data->id;
	params = data->params;
	while (1)
	{
		think(id);
		eat(id);
		sleep(id);
	}
	return (NULL);
}
