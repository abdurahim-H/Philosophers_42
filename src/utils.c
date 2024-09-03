#include "philo.h"

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	safe_print(t_params *params, int id, const char *message)
{
	pthread_mutex_lock(&params->end_mutex);
	if (!params->simulation_end)
		printf("%lld %d %s\n", get_current_time(), id, message);
	pthread_mutex_unlock(&params->end_mutex);
}

int	safe_mutex_operation(pthread_mutex_t *mutex, int lock)
{
	int	result;

	if (lock)
		result = pthread_mutex_lock(mutex);
	else
		result = pthread_mutex_unlock(mutex);
	if (result != 0)
	{
		printf("Error: mutex operation failed\n");
		return (1);
	}
	return (0);
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

void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}
