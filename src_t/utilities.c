/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abhudulo <abhudulo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 03:45:36 by abhudulo          #+#    #+#             */
/*   Updated: 2024/08/03 09:03:02 by abhudulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	log_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
}
