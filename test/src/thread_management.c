#include "philo.h"

int create_philo_thread(pthread_t *thread, t_philo_data *data,
    void *(*philo_lifecycle)(void *))
{
    if (pthread_create(thread, NULL, philo_lifecycle, data) != 0)
    {
        log_error("Unable to create thread");
        return (1);
    }
    return (0);
}

int create_philosophers(pthread_t *threads, int num_philosophers,
    void *(*philo_lifecycle)(void *), t_philo_data **philo_data)
{
    int i;

    i = 0;
    while (i < num_philosophers)
    {
        if (create_philo_thread(&threads[i], philo_data[i],
                philo_lifecycle) != 0)
        {
            cleanup_exit(threads, philo_data, i);
            return (1);
        }
        i++;
    }
    return (0);
}

void join_and_free_threads(pthread_t *threads, t_philo_data **philo_data,
        int num_philosophers)
{
    int i;

    i = 0;
    while (i < num_philosophers)
    {
        pthread_join(threads[i], NULL);
        free(philo_data[i]);
        i++;
    }
    free(threads);
    free(philo_data);
}

void cleanup_exit(pthread_t *threads, t_philo_data **philo_data,
        int num_created)
{
    int i;

    i = 0;
    while (i < num_created)
    {
        pthread_cancel(threads[i]);
        pthread_join(threads[i], NULL);
        free(philo_data[i]);
        i++;
    }
    free(philo_data);
    free(threads);
}