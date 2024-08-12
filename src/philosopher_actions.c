#include "philo.h"

void	think(int philosopher_id)
{
	printf("Philosopher %d is thinking\n", philosopher_id);
	usleep((rand() % 1000 + 1) * 1000);
}

// void	eat(t_philo_data *philo, int left_fork, int right_fork)
// {
// 	struct timeval		tv;
// 	long long			current_time;

// 	pthread_mutex_lock(&(philo->params->forks[right_fork]));
// 	pthread_mutex_lock(&(philo->params->forks[left_fork]));
// 	gettimeofday(&tv, NULL);
// 	current_time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
// 	safe_mutex_operation(&philo->data_mutex, 1);
// 	philo->philosopher.state = EATING;
// 	philo->philosopher.last_meal_time = current_time;
// 	philo->philosopher.meals_eaten += 1;
// 	safe_mutex_operation(&philo->data_mutex, 0);
// 	printf("Philosopher %d is eating\n", philo->id);
// 	usleep(philo->params->time_to_eat * 1000);
// 	pthread_mutex_unlock(&(philo->params->forks[right_fork]));
// 	pthread_mutex_unlock(&(philo->params->forks[left_fork]));
// }

bool    eat(t_philo_data *philo, int left_fork, int right_fork)
{
    struct timeval      tv;
    long long           current_time;

    pthread_mutex_lock(&(philo->params->forks[right_fork]));
    pthread_mutex_lock(&(philo->params->forks[left_fork]));
    
    pthread_mutex_lock(&philo->params->end_mutex);
    if (philo->params->simulation_end)
    {
        pthread_mutex_unlock(&philo->params->end_mutex);
        pthread_mutex_unlock(&(philo->params->forks[left_fork]));
        pthread_mutex_unlock(&(philo->params->forks[right_fork]));
        return (false);
    }
    pthread_mutex_unlock(&philo->params->end_mutex);

    gettimeofday(&tv, NULL);
    current_time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    safe_mutex_operation(&philo->data_mutex, 1);
    philo->philosopher.state = EATING;
    philo->philosopher.last_meal_time = current_time;
    philo->philosopher.meals_eaten += 1;
    safe_mutex_operation(&philo->data_mutex, 0);
    printf("Philosopher %d is eating\n", philo->id);
    usleep(philo->params->time_to_eat * 1000);
    pthread_mutex_unlock(&(philo->params->forks[right_fork]));
    pthread_mutex_unlock(&(philo->params->forks[left_fork]));
    return (true);
}

void	ft_sleep(int philosopher_id, t_params *params)
{
	printf("Philosopher %d is sleeping\n", philosopher_id);
	usleep(params->time_to_sleep * 1000);
}

// bool	is_philosopher_alive(t_philo_data *philo)
// {
// 	long long			current_time;
// 	long long			time_since_last_meal;
// 	long long			last_meal_time;
// 	int					lock;

// 	lock = 1;
// 	current_time = get_current_time();
// 	if (safe_mutex_operation(&philo->data_mutex, lock) != 0)
// 		return (false);
// 	last_meal_time = philo->philosopher.last_meal_time;
// 	safe_mutex_operation(&philo->data_mutex, !lock);
// 	time_since_last_meal = current_time - last_meal_time;
// 	if (time_since_last_meal > philo->params->time_to_die)
// 		return (false);
// 	return (true);
// }

bool    is_philosopher_alive(t_philo_data *philo)
{
    long long           current_time;
    long long           time_since_last_meal;
    long long           last_meal_time;
    int                 lock;

    lock = 1;
    current_time = get_current_time();
    if (safe_mutex_operation(&philo->data_mutex, lock) != 0)
        return (false);
    last_meal_time = philo->philosopher.last_meal_time;
    safe_mutex_operation(&philo->data_mutex, !lock);
    time_since_last_meal = current_time - last_meal_time;
    if (time_since_last_meal > philo->params->time_to_die)
    {
        pthread_mutex_lock(&philo->params->end_mutex);
        if (!philo->params->simulation_end)
        {
            printf("Philosopher %d has died\n", philo->id);
            philo->params->simulation_end = 1;
        }
        pthread_mutex_unlock(&philo->params->end_mutex);
        return (false);
    }
    return (true);
}

// void	*philo_lifecycle(void *arg)
// {
// 	t_philo_data	*data;
// 	int				left_fork;
// 	int				right_fork;

// 	data = (t_philo_data *)arg;
// 	right_fork = data->id - 1;
// 	left_fork = (data->id % data->params->num_philosophers);
// 	while (1)
// 	{
// 		pthread_mutex_lock(&data->params->end_mutex);
// 		if (data->params->simulation_end)
// 		{
// 			pthread_mutex_unlock(&data->params->end_mutex);
// 			break ;
// 		}
// 		pthread_mutex_unlock(&data->params->end_mutex);
// 		if (!is_philosopher_alive(data))
// 			break ;
// 		philosopher_actions(data, left_fork, right_fork);
// 	}
// 	return (NULL);
// }

void    *philo_lifecycle(void *arg)
{
    t_philo_data    *data;
    int             left_fork;
    int             right_fork;

    data = (t_philo_data *)arg;
    right_fork = data->id - 1;
    left_fork = (data->id % data->params->num_philosophers);
    while (1)
    {
        pthread_mutex_lock(&data->params->end_mutex);
        if (data->params->simulation_end)
        {
            pthread_mutex_unlock(&data->params->end_mutex);
            break ;
        }
        pthread_mutex_unlock(&data->params->end_mutex);
        if (!is_philosopher_alive(data))
            break ;
        if (!philosopher_actions(data, left_fork, right_fork))
            break ;
    }
    return (NULL);
}