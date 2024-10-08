/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abhudulo <abhudulo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 21:16:18 by abhudulo          #+#    #+#             */
/*   Updated: 2024/09/03 15:49:29 by abhudulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>
# include <sys/time.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_params
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	pthread_mutex_t	*forks;
	int				simulation_end;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	print_mutex;
}	t_params;

typedef enum e_state
{
	THINKING,
	EATING,
	SLEPING,
}	t_state;

typedef struct s_philosopher
{
	int				id;
	t_state			state;
	long long		last_meal_time;
	int				meals_eaten;
}	t_philosopher;

typedef struct s_philo_data
{
	int				id;
	t_params		*params;
	t_philosopher	philosopher;
	pthread_mutex_t	data_mutex;
}	t_philo_data;

typedef struct s_monitor_data
{
	t_params		*params;
	t_philo_data	**philo_data;
}	t_monitor_data;

typedef struct s_sim_data
{
	t_params		*params;
	pthread_t		*threads;
	t_philo_data	**philo_data;
	pthread_t		monitor_thread;
}	t_sim_data;

int				my_atoi(const char *str);
void			log_error(const char *message);
void			ft_sleep(t_philo_data *philo);
void			eat(t_philo_data *philo);
void			think(t_philo_data *philo);
void			*philo_lifecycle(void *arg);
int				validate_params(t_params *params);
int				parse_arguments(int argc, char *argv[], t_params *params);
int				allocate_threads(pthread_t **threads, int num_philosophers);
void			cleanup_threads(pthread_t *threads, int **ids, int i);
t_philo_data	**allocate_philo_data(int num_philosophers, t_params *params);
int				create_philo_thread(pthread_t *thread, t_philo_data *data,
					void *(*philo_lifecycle)(void *));
void			cleanup_exit(pthread_t *threads, t_philo_data **philo_data,
					int num_created);
int				create_philosophers(pthread_t *threads, int num_philosophers,
					void *(*philo_lifecycle)(void *),
					t_philo_data **philo_data);
void			join_and_free_threads(pthread_t *threads,
					t_philo_data **philo_data, int num_philosophers);
int				main(int argc, char **argv);
int				initialize_mutexes(t_params *params);
int				initialize_simulation(t_params *params, pthread_t **threads,
					t_philo_data ***philo_data);
void			cleanup_simulation(t_params *params, pthread_t *threads,
					t_philo_data **philo_data);
void			cleanup_mutexes(t_params *params);
void			initialize_philosopher(t_philo_data *philo, int id,
					t_params *params);
long long		get_current_time(void);
bool			is_philosopher_alive(t_philo_data *philo);
void			philosopher_actions(t_philo_data *data, int left_fork,
					int right_fork);
int				check_philosopher_status(t_monitor_data *data,
					int *philosophers_done);
void			*monitor_philosophers(void *arg);
int				initialize_all(int argc, char **argv, t_sim_data *sim_data);
int				create_all_threads(t_sim_data *sim_data);
void			join_and_cleanup(t_sim_data *sim_data);
int				safe_mutex_operation(pthread_mutex_t *mutex, int lock);
int				handle_simulation_end_condition(t_monitor_data *data);
void			cleanup_philosopher_mutexes(t_philo_data **philo_data,
					int num_philosophers);
int				check_simulation_end(t_params *params);
int				take_forks(t_philo_data *data, int left_fork, int right_fork);
void			release_forks(t_philo_data *data, int left_fork, int right_fork);
int				handle_single_philosopher(t_philo_data *data, int fork);
int				take_first_fork(t_philo_data *data, int fork);
int				take_second_fork(t_philo_data *data, int first, int second);
int				init_end_mutex(t_params *params);
int				init_forks_and_threads(t_params *params, pthread_t **threads);
int				init_philo_data(t_params *params, pthread_t **threads,
					t_philo_data ***philo_data);
// int				check_philosophers(t_monitor_data *data, int i);
int 			check_philosophers(t_monitor_data *data);
int				check_all_philosophers_finished(t_monitor_data *data);
int				check_philosopher_death(t_monitor_data *data, int i);
int				count_philosophers_done_eating_loop(t_monitor_data *data,
					int *philosophers_done);
int				convert_str_to_int(const char *str, int *sign);
int				handle_overflow(int result, char current_char, int sign);
int				check_all_philosophers_alive(t_monitor_data *data);
int				check_execution_limit(void);
void			safe_mutex_destroy(pthread_mutex_t *mutex);
void			safe_free(void **ptr);
int				initialize_philosophers(t_params *params, t_philo_data ***philo_data,
					pthread_t **threads);
#endif