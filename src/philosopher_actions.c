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
	int	philosopher_id;

	philosopher_id = *(int *)arg;
	while (1)
	{
		think(philosopher_id);
		eat(philosopher_id);
		sleep(philosopher_id);
	}
	return (NULL);
}
