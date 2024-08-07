#include "philo.h"

int validate_params(t_params *params)
{
    if (params->num_philosophers <= 0 || params->time_to_die <= 0
        || params->time_to_eat <= 0 || params->time_to_sleep <= 0
        || (params->num_times_to_eat <= 0 && params->num_times_to_eat != -1))
    {
        log_error("Invalid argument values");
        return (-1);
    }
    if (params->num_philosophers > INT_MAX
        || params->num_philosophers < INT_MIN)
        return (-1);
    if (params->time_to_die > INT_MAX || params->time_to_die < INT_MIN)
        return (-1);
    if (params->time_to_eat > INT_MAX || params->time_to_eat < INT_MIN)
        return (-1);
    if (params->time_to_sleep > INT_MAX || params->time_to_sleep < INT_MIN)
        return (-1);
    if (params->num_times_to_eat > INT_MAX
        || params->num_times_to_eat < INT_MIN)
        return (-1);
    return (0);
}

int parse_arguments(int argc, char *argv[], t_params *params)
{
    if (argc != 5 && argc != 6)
    {
        log_error("Invalid number of arguments");
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