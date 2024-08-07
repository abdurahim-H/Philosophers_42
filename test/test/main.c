#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_PHILOS 200

typedef struct s_philo
{
    int             id;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct timeval  last_meal;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             num_times_to_eat;
    int             is_dead;
}               t_philo;

typedef struct s_data
{
    int             num_philos;
    pthread_mutex_t *forks;
    t_philo         *philos;
    pthread_mutex_t death_mutex;
}               t_data;

int         init_data(t_data *data, int argc, char **argv)
{
    data->num_philos = atoi(argv[1]);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
    data->philos = malloc(sizeof(t_philo) * data->num_philos);
    pthread_mutex_init(&data->death_mutex, NULL);
    for (int i = 0; i < data->num_philos; i++)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        data->philos[i].id = i + 1;
        data->philos[i].left_fork = &data->forks[i];
        data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
        data->philos[i].time_to_die = atoi(argv[2]);
        data->philos[i].time_to_eat = atoi(argv[3]);
        data->philos[i].time_to_sleep = atoi(argv[4]);
        if (argc == 6)
            data->philos[i].num_times_to_eat = atoi(argv[5]);
        else
            data->philos[i].num_times_to_eat = -1;
        data->philos[i].is_dead = 0;
    }
    return (0);
}

void        *philo_thread(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = (t_data *)philo->left_fork;
    while (1)
    {
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(philo->right_fork);
        eat(philo);
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        sleep_philo(philo);
        think(philo);
        if (philo->num_times_to_eat != -1 && philo->num_times_to_eat == 0)
            break;
    }
    return (NULL);
}

void        eat(t_philo *philo)
{
    t_data *data = (t_data *)philo->left_fork;
    pthread_mutex_lock(&data->death_mutex);
    if (philo->is_dead)
    {
        pthread_mutex_unlock(&data->death_mutex);
        return;
    }
    philo->last_meal = get_time();
    pthread_mutex_unlock(&data->death_mutex);
    print_status(philo, "is eating");
    usleep(philo->time_to_eat * 1000);
    if (philo->num_times_to_eat != -1)
        philo->num_times_to_eat--;
}

void        sleep_philo(t_philo *philo)
{
    print_status(philo, "is sleeping");
    usleep(philo->time_to_sleep * 1000);
}

void        think(t_philo *philo)
{
    print_status(philo, "is thinking");
}

int         get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void        print_status(t_philo *philo, char *status)
{
    t_data *data = (t_data *)philo->left_fork;
    pthread_mutex_lock(&data->death_mutex);
    if (philo->is_dead)
    {
        pthread_mutex_unlock(&data->death_mutex);
        return;
    }
    printf("%d %d %s\n", get_time(), philo->id, status);
    pthread_mutex_unlock(&data->death_mutex);
}

void        check_death(t_data *data)
{
    for (int i = 0; i < data->num_philos; i++)
    {
        pthread_mutex_lock(&data->death_mutex);
        if (data->philos[i].is_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            continue;
        }
        if