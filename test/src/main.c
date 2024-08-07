#include "philo.h"

int initialize_all(int argc, char **argv, t_sim_data *sim_data)
{
    if (parse_arguments(argc, argv, sim_data->params) != 0)
        return (1);
    if (initialize_simulation(sim_data->params,
            &sim_data->threads, &sim_data->philo_data) != 0)
    {
        return (1);
    }
    return (0);
}

int create_all_threads(t_sim_data *sim_data)
{
    t_monitor_data *monitor_data;
    int i;

    monitor_data = malloc(sizeof(t_monitor_data));
    if (!monitor_data)
        return (1);
    monitor_data->params = sim_data->params;
    monitor_data->philo_data = sim_data->philo_data;
    if (pthread_create(&sim_data->monitor_thread, NULL, monitor_philosophers,
            monitor_data) != 0)
    {
        free(monitor_data);
        log_error("Failed to create monitor thread");
        return (1);
    }
    i = 0;
    while (i < sim_data->params->num_philosophers)
    {
        if (pthread_create(&sim_data->threads[i], NULL, philo_lifecycle,
                sim_data->philo_data[i]) != 0)
        {
            log_error("Failed to create philosopher thread");
            return (1);
        }
        i++;
    }
    return (0);
}

void join_and_cleanup(t_sim_data *sim_data)
{
    int i;

    i = 0;
    while (i < sim_data->params->num_philosophers)
    {
        pthread_join(sim_data->threads[i], NULL);
        i++;
    }
    pthread_join(sim_data->monitor_thread, NULL);
    cleanup_simulation(sim_data->params, sim_data->threads, sim_data->philo_data);
}

int main(int argc, char **argv)
{
    t_sim_data sim_data;
    t_params params;

    sim_data.params = &params;
    if (initialize_all(argc, argv, &sim_data) != 0)
        return (1);
    if (create_all_threads(&sim_data) != 0)
    {
        cleanup_simulation(sim_data.params, sim_data.threads,
            sim_data.philo_data);
        return (1);
    }
    join_and_cleanup(&sim_data);
    return (0);
}