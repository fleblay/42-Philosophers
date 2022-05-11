/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fred <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:32:42 by fred              #+#    #+#             */
/*   Updated: 2022/05/11 15:50:00 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>
#include <stdio.h>

void	ft_grab_forks(t_data *data)
{
	sem_wait(data->s_fork);
	ft_safe_print(data, "has taken a fork", 0);
	sem_wait(data->s_fork);
	ft_safe_print(data, "has taken a fork", 0);
}

void	ft_give_back_forks(t_data *data)
{
	sem_post(data->s_fork);
	if (data->philo_count > 1)
		sem_post(data->s_fork);
}

int	ft_eat(t_data *data)
{
	if (ft_sim_is_over(data))
		return (ft_give_back_forks(data), 0);
	data->current_time = ft_get_time() - data->start_time;
	sem_wait(data->s_self_dead[data->id]);
	data->last_start_eat = data->current_time;
	sem_post(data->s_self_dead[data->id]);
	ft_safe_print(data, "is eating", 0);
	while (data->current_time - data->last_start_eat < data->tte)
	{
		if (ft_sim_is_over(data))
			return (ft_give_back_forks(data), 0);
		else
			usleep(500);
		data->current_time = ft_get_time() - data->start_time;
	}
	ft_give_back_forks(data);
	data->meal_goal--;
	if (!data->meal_goal)
		sem_post(data->s_meal);
	return (1);
}

int	ft_sleep(t_data *data)
{
	data->current_time = ft_get_time() - data->start_time;
	data->last_start_sleep = data->current_time;
	ft_safe_print(data, "is sleeping", 0);
	while (data->current_time - data->last_start_sleep < data->tts)
	{
		if (ft_sim_is_over(data))
			return (0);
		else
			usleep(500);
		data->current_time = ft_get_time() - data->start_time;
	}
	return (1);
}

int	ft_think(t_data *data)
{
	data->current_time = ft_get_time() - data->start_time;
	data->last_start_think = data->current_time;
	ft_safe_print(data, "is thinking", 0);
	if (data->ttd < data->tte + data->tts + 10)
		return (1);
	while (data->current_time - data->last_start_think
		< data->ttd - data->tte - data->tts - 10)
	{
		if (ft_sim_is_over(data))
			return (0);
		else
			usleep(500);
		data->current_time = ft_get_time() - data->start_time;
	}
	return (1);
}
