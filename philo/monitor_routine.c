/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:29:13 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/28 17:46:50 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

void	get_request(t_data *data)
{
	data->request_pending = -1;
	while (data->run && data->request_pending == -1)
	{
		pthread_mutex_lock(&data->server_request);
		if (data->request == -1)
		{
			if (monitor_someone_is_dead(data))
				data->run = 0;
			DEBUG && safe_print(-1, "DEBUG && Monitor waiting for request\n", &data->print, data);
			pthread_mutex_unlock(&data->server_request);
			usleep(20);
			continue ;
		}
		data->request_pending = data->request;
		data->request = -1;
		DEBUG && safe_print(data->request_pending, "DEBUG && Monitor has a request\n", &data->print, data);
		pthread_mutex_unlock(&data->server_request);
	}
}

int	check_available_forks(t_data *data)
{
	int	available;
	int	index_fork1;
	int	index_fork2;

	if (data->request_pending < 1)
		return (0);
	available = 0;
	index_fork1 = data->request_pending - 1;
	index_fork2 = data->request_pending % data->philo_count;
	if (index_fork1 == index_fork2)
		return (0);
	pthread_mutex_lock(&data->fork[index_fork1]);
	available += data->fork_available[index_fork1];
	pthread_mutex_unlock(&data->fork[index_fork1]);
	pthread_mutex_lock(&data->fork[index_fork2]);
	available += data->fork_available[index_fork2];
	pthread_mutex_unlock(&data->fork[index_fork2]);
	return ((available == 2) * 1);
}

int	meal_goal_achieved(t_data *data)
{
	int i;

	i = 0;
	if (data->ntepme == -1)
		return (0);
	pthread_mutex_lock(&data->meal);
	while (i < data->philo_count)
	{
		if (data->meal_count[i] < data->ntepme)
		{
			pthread_mutex_unlock(&data->meal);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&data->meal);
	data->meal_goal_achieved = 1;
	return (1);
}
