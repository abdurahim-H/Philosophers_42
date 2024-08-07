#include "philo.h"

int handle_overflow(int result, char current_char, int sign)
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

int convert_str_to_int(const char *str, int *sign)
{
    int result;
    int i;

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

int my_atoi(const char *str)
{
    int sign;
    int i;

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


long long get_current_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void log_error(const char *message)
{
    printf("Error: %s\n", message);
}

void print_status(t_params *params, int id, char *status)
{
    long long current_time;

    pthread_mutex_lock(&params->write_mutex);
    if (!params->simulation_end)
    {
        current_time = get_current_time();
        printf("%lld %d %s\n", current_time, id, status);
    }
    pthread_mutex_unlock(&params->write_mutex);
}

int safe_mutex_operation(pthread_mutex_t *mutex, int lock)
{
    if (lock)
        return (pthread_mutex_lock(mutex));
    else
        return (pthread_mutex_unlock(mutex));
}