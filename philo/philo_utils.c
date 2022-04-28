/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 14:09:27 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/28 15:08:03 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

int		get_sim_duration(void)
{
	static time_t		start_sec = 0;
	static suseconds_t	start_usec = 0;
	struct timeval		time;
	int					time_stamp;

	if (start_sec == 0 && start_usec == 0)
	{
		if (gettimeofday(&time, NULL) == -1)
			return (-1);
		start_sec = time.tv_sec;
		start_usec = time.tv_usec;
		printf("Setting start of sim [%ld:%ld]\n", start_sec, (long)start_usec);
		return (0);
	}
	else if (gettimeofday(&time, NULL) == -1)
		return (-1);
	time_stamp = (time.tv_sec - start_sec) * 1000
		+ (time.tv_usec - start_usec) / 1000;
	return (time_stamp);
}

int	safe_print(int id, char *txt, pthread_mutex_t *print, int monitor)
{
	(void)monitor;
	(void)print;
	pthread_mutex_lock(print);
	printf("%d %d %s", get_sim_duration(), id, txt);
	pthread_mutex_unlock(print);
	return (1);
}
/*
void	safe_print(int id, char *txt, pthread_mutex_t *print, int monitor)
{
	pthread_mutex_lock(print);
	printf("[%d] ", get_sim_duration());
	if (monitor)
		printf("[MONITOR]");
	if (id >= 0)
		printf("Philo %d ", id);
	printf("%s", txt);
	pthread_mutex_unlock(print);
}
*/

int	self_is_dead(t_philo *philo)
{
	if (get_sim_duration() - philo->start_eat > philo->data->ttd)
	{
		DEBUG && safe_print(philo->id, "DEBUG && I am dead\n", &philo->data->print, 0);
		pthread_mutex_lock(&philo->data->server_dead_philo);
		philo->data->dead_philo = philo->id;
		philo->dead = philo->id;
		pthread_mutex_unlock(&philo->data->server_dead_philo);
		return (1);
	}
	return (0);
}

int	someone_is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->server_dead_philo);
	if (philo->data->dead_philo != -1)
	{
		philo->dead = philo->data->dead_philo;
		pthread_mutex_unlock(&philo->data->server_dead_philo);
		DEBUG && safe_print(philo->id, "DEBUG && Found out dead philo\n", &philo->data->print, 0);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->server_dead_philo);
	return (0);
}
