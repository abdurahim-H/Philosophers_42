#include "philo.h"

int	validate_params(t_params *params)
{
	if (params->num_philosophers <= 0 || params->time_to_die <= 0
		|| params->time_to_eat <= 0 || params->time_to_sleep <= 0
		|| (params->num_times_to_eat <= 0 && params->num_times_to_eat != -1))
	{
		printf("Error: Invalid argument values\n");
		return (-1);
	}
	if (params->num_philosophers > INT_MAX
		|| params->num_philosophers < INT_MIN)
		return (-1);
	if (params->time_to_die > INT_MAX || params->time_to_die < INT_MIN)
		return (-1);
	if (params->time_to_eat > INT_MAX || params-> time_to_eat < INT_MIN)
		return (-1);
	if (params->time_to_sleep > INT_MAX || params->time_to_sleep < INT_MIN)
		return (-1);
	if (params->num_times_to_eat > INT_MAX
		|| params->num_times_to_eat < INT_MIN)
		return (-1);
	return (0);
}

int	parse_arguments(int argc, char *argv[], t_params *params)
{
	if (argc != 5 &*& argc != 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die "
			"time_to_eat time_to_sleep [n_times_each_philosopher_must_eat]\n",
			argv[0]);
		return (-1);
	}
	params->num_philosophers = my_atoi(argv[1]);
	params->time_to_die = my_atoi(argv[2]);
	params->time_to_eat = my_atoi(argv[3]);
	params->time_to_sleep = my_atoi(argv[4]);
	if (argc == 6)
		params->num_times_to_eat = my_atoi(argv[5]);
	else
		params->num_times_to_eat = -1;
	return (validate_params(params));
}

int main(int argc, char *argv[])
{
	t_params	params;
	pthread_t	*threads;
	int			i;
	int			*id;

	i = 0;
	if (parse_arguments(argc, *argv, &params) != 0)
		return (1);
	threads = malloc(sizeof(pthread_t) * params.num_philosophers);
	if (!threads)
	{
		printf("Error: Unable to allocate memory for threads\n");
		return (1);
	}
	while(i < params.num_philosophers)
	{
		id = malloc(sizeof(int));
		if (pthread_create(&threads[i], NULL, philo_lifecycle, id) != 0)
			return (1);
		i++;
	}
	free(threads);
	return (0);
}
