/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abhudulo <abhudulo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 21:16:18 by abhudulo          #+#    #+#             */
/*   Updated: 2024/07/21 02:56:13 by abhudulo         ###   ########.fr       */
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

typedef struct s_params
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	pthread_mutex_t	*forks;
}	t_params;

typedef enum e_state
{
	THINKING,
	EATING,
	SLEPING,
}	t_state;

typedef struct s_philosopher
{
	int			id;
	t_state		state;
	long long	last_meal_time;
	int			meals_eaten;
}	t_philosopher;

typedef struct s_philo_data
{
	int			id;
	t_params	*params;
}	t_philo_data;

int				my_atoi(const char *str);
void			sleep(int philosopher_id);
void			eat(int philosopher_id);
void			think(int philosopher_id);
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
					void *(*philo_lifecycle)(void *), t_philo_data **philo_data);
void			join_and_free_threads(pthread_t *threads, t_philo_data **philo_data,
					int num_philosophers);
int				main(int argc, char **argv);
int				initialize_mutexes(t_params *params);
#endif