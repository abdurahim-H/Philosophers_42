/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abhudulo <abhudulo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 21:16:18 by abhudulo          #+#    #+#             */
/*   Updated: 2024/07/09 06:30:23 by abhudulo         ###   ########.fr       */
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

typedef struct s_params
{
	int			num_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_times_to_eat;
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

int		my_atoi(const char *str);
void	sleep(int philosopher_id);
void	eat(int philosopher_id);
void	think(int philosopher_id);
void	*philo_lifecycle(void *arg);

#endif