/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 13:15:50 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/11 10:29:48 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stddef.h>
#include "philo_bonus.h"
#include <stdio.h>

unsigned long	ft_get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

int		ft_self_is_dead(t_data *data)
{
	unsigned long	current_time;

	sem_wait(data->s_self_dead[data->id]);
	if (data->start_time == 0)
	{
		sem_post(data->s_self_dead[data->id]);
		return (0);
	}
	current_time = ft_get_time() - data->start_time;
	if (current_time - data->last_start_eat > data->ttd)
	{
		sem_post(data->s_self_dead[data->id]);
		sem_post(data->s_dead_signal);
		ft_safe_print(data, "died", 1);
		return (1);
	}
	sem_post(data->s_self_dead[data->id]);
	return (0);
}
