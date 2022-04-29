/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 14:09:27 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/29 12:38:49 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

int		get_time(void)
{
	static int count = 0;
	struct timeval	time;
	static int		last_ret = 0;
	int				ret;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	count++;
	ret = time.tv_sec * 1000 + time.tv_usec / 1000;
	/*
	if (last_ret != 0 && ret - last_ret > 5)
		fprintf(stdout, "\x1b[31mHUGE GAP\x1b[0m : %d\n", ret - last_ret);
	*/
	last_ret = ret;
	//fprintf(stderr, "Call count [%5d] | Value [%d]\n", count, ret);
	return (ret);
}

/*
int		get_sim_duration(int id, char *txt)
{
	static time_t		start_sec = 0;
	static suseconds_t	start_usec = 0;
	struct timeval		time;
	int					time_stamp;

	if (start_sec == 0 && start_usec == 0)
	{
		start_sec = time.tv_sec;
		start_usec = time.tv_usec;
		printf("Setting start of sim [%ld:%ld]\n", start_sec, (long)start_usec);
		return (start_sec * 1000 + start_usec / 1000);
	}
	else if (gettimeofday(&time, NULL) == -1)
		return (-1);
	time_stamp = (time.tv_sec - start_sec) * 1000
		+ (time.tv_usec - start_usec) / 1000;
	fprintf(stderr, "Philo %d : Use %s [%d]\n",id, txt, time_stamp);
	return (time_stamp);
}
*/

int	safe_print_monitor(char *txt, t_data *data)
{
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->time_mutex);
	printf("%d MONITOR %s", get_time() - data->start_time, txt);
	pthread_mutex_unlock(&data->time_mutex);
	pthread_mutex_unlock(&data->print);
	return (1);
}

int	safe_print(char *txt, t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->time_mutex);
	printf("%d %d %s", get_time() - philo->start_time, philo->id, txt);
	pthread_mutex_unlock(&philo->data->time_mutex);
	pthread_mutex_unlock(&philo->data->print);
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

	pthread_mutex_lock(&philo->data->time_mutex);
	philo->current_time = get_time() - philo->start_time;
	pthread_mutex_unlock(&philo->data->time_mutex);
	if (philo->current_time - philo->start_eat > philo->ttd)
	{
		DEBUG && safe_print("DEBUG && I am dead\n", philo);
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
		DEBUG && safe_print("DEBUG && Found out dead philo\n", philo);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->server_dead_philo);
	return (0);
}
