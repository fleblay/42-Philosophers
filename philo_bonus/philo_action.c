/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fred <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:32:42 by fred              #+#    #+#             */
/*   Updated: 2022/05/10 17:57:36 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>
#include <stdio.h>

int	ft_grab_forks(t_data *data)
{
	ft_safe_print(data, "waiting for 1st fork", 0);
	sem_wait(data->s_fork);
	ft_safe_print(data, "has taken a fork", 0);
	ft_safe_print(data, "waiting for 2nd fork", 0);
	sem_wait(data->s_fork);
	ft_safe_print(data, "has taken a fork", 0);
	ft_safe_print(data, "success getting 2 forks", 0);
	return (1);
}

int	ft_eat(t_data *data)
{
		check_if_end_sim(data);
		if (data->go_on == 0)
		{
			printf("givin back 2 forks %d\n", data->id + 1);
			sem_post(data->s_fork);
			if (data->philo_count > 1)
				sem_post(data->s_fork);
			return (0);
		}
	data->current_time = ft_get_time() - data->start_time;
	// utiliser un semaphore prore au lieu de partager celui de data->dead ?
	data->last_start_eat = data->current_time;
	sem_wait(data->s_self_dead[data->id]);
	data->last_start_eat2 = data->last_start_eat;
	sem_post(data->s_self_dead[data->id]);
	// utiliser un semaphore prore au lieu de partager celui de data->dead ?
	ft_safe_print(data, "is eating", 0);
	while (data->current_time - data->last_start_eat < data->tte)
	{
		check_if_end_sim(data);
		if (data->go_on == 0)
		{
			//printf("givin back 2 forks %d\n", data->id + 1);
			sem_post(data->s_fork);
			if (data->philo_count > 1)
				sem_post(data->s_fork);
			return (0);
		}
		else
			usleep(500);
		data->current_time = ft_get_time() - data->start_time;
	}
	//printf("givin back 2 forks %d\n", data->id + 1);
	sem_post(data->s_fork);
			if (data->philo_count > 1)
				sem_post(data->s_fork);
	data->meal_goal--;
	if (!data->meal_goal)
	{
		//printf("meal goal achieved %d\n", data->id + 1);
		sem_post(data->s_meal);
	}
	return (1);
}

int	ft_sleep(t_data *data)
{
	data->current_time = ft_get_time() - data->start_time;
	data->last_start_sleep = data->current_time;
	ft_safe_print(data, "is sleeping", 0);
	while (data->current_time - data->last_start_sleep < data->tts)
	{
		check_if_end_sim(data);
		if (data->go_on == 0)
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
		check_if_end_sim(data);
		if (data->go_on == 0)
			return (0);
		else
			usleep(500);
		data->current_time = ft_get_time() - data->start_time;
	}
	return (1);
}

