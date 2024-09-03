#include "philo.h"

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

int	validate_params(t_params *params)
{
	if (params->num_philosophers <= 0 || params->time_to_die <= 0
		|| params->time_to_eat <= 0 || params->time_to_sleep <= 0
		|| (params->num_times_to_eat <= 0 && params->num_times_to_eat != -1))
	{
		printf("ERROR: INVALID ARGUMENT VALUES\n");
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

int	my_atoi(const char *str)
{
	int	sign;
	int	i;

	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	return (convert_str_to_int(&str[i], &sign));
}

int	convert_str_to_int(const char *str, int *sign)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (handle_overflow(result, str[i], *sign) != 1)
			return (handle_overflow(result, str[i], *sign));
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return (*sign * result);
}

int	handle_overflow(int result, char current_char, int sign)
{
	if (result > INT_MAX / 10 || (result == INT_MAX / 10
			&& current_char - '0' > INT_MAX % 10))
	{
		if (sign == 1)
			return (-1);
		else
			return (0);
	}
	return (1);
}
