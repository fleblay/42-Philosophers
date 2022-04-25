/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:17:42 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/25 13:15:20 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

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
		printf("Setting start of sim [%ld:%ld]\n", start_sec, start_usec);
		return (0);
	}
	else if (gettimeofday(&time, NULL) == -1)
		return (-1);
	time_stamp = 
}

void	safe_print(int id, char *txt, pthread_mutex_t *print, int monitor)
{
	pthread_mutex_lock(print);
	if (monitor)
		printf("[MONITOR]");
	if (id >= 0)
		printf("Philo %d ", id);
	printf("%s", txt);
	pthread_mutex_unlock(print);
}

void	*philo_routine(void *phil)
{
	t_philo	*philo;
	philo = (t_philo *)phil;
	while (philo->dead == -1)
	{
		pthread_mutex_lock(&philo->data->server_com);
		safe_print(philo->id, "Com ok\n", &philo->data->print, 0);
		pthread_mutex_lock(&philo->data->server_request);
		philo->data->request = philo->id;
		safe_print(philo->id, "Request made\n", &philo->data->print, 0);
		pthread_mutex_unlock(&philo->data->server_request);
		philo->answer = -1;
		while (philo->dead == -1 && philo->answer == -1)
		{
			pthread_mutex_lock(&philo->data->server_answer);
			if (philo->data->answer == -1)
			{
				safe_print(philo->id, "Waiting for answer\n", &philo->data->print, 0);
				pthread_mutex_unlock(&philo->data->server_answer);
				//check if self dead
				usleep(20);
				continue ;
			}
			safe_print(philo->id, "Answer is given after waiting\n", &philo->data->print, 0);
			philo->answer = philo->data->answer;
			philo->data->answer = -1;
			pthread_mutex_unlock(&philo->data->server_answer);
		}
		safe_print(philo->id, "Answer is given\n", &philo->data->print, 0);
		pthread_mutex_unlock(&philo->data->server_com);
		safe_print(philo->id, "Releasing com\n", &philo->data->print, 0);
		if (philo->answer == 0)
		{
			safe_print(philo->id, "answer is KO\n", &philo->data->print, 0);
			usleep(10);
			safe_print(philo->id, "Finished waiting after KO\n", &philo->data->print, 0);
		}
		else
		{
			safe_print(philo->id, "answer is OK\n", &philo->data->print, 0);
			usleep(2000);
			safe_print(philo->id, "Finished waiting after OK\n", &philo->data->print, 0);
		}
		//check if self dead
	}
	return (phil);
}

int	main(void)
{
	long		i;
	static int	ok = 0;
	t_data	data;
	t_philo	philo[3];

	pthread_mutex_init(&data.fork1, NULL);
	pthread_mutex_init(&data.fork2, NULL);
	pthread_mutex_init(&data.fork3, NULL);
	pthread_mutex_init(&data.server_request, NULL);
	pthread_mutex_init(&data.server_answer, NULL);
	pthread_mutex_init(&data.server_com, NULL);
	pthread_mutex_init(&data.server_dead_philo, NULL);
	pthread_mutex_init(&data.print, NULL);
	data.request_pending = -1;
	data.request = -1;
	data.answer = -1;
	data.dead_philo = -1;
	data.run = 1;

	i = 0;
	get_sim_duration();
	while (i < 3)
	{
		philo[i].id = i;
		philo[i].answer = -1;
		philo[i].dead = -1;
		philo[i].data = &data;
		pthread_create(&data.thread[i], NULL, philo_routine, &philo[i]);
		i++;
	}
	while (data.run)
	{
		data.request_pending = -1;
		while (data.run && data.request_pending == -1)
		{
			pthread_mutex_lock(&data.server_request);
			if (data.request == -1)
			{
				safe_print(-1, "Monitor waiting for request\n", &data.print, 1);
				pthread_mutex_unlock(&data.server_request);
				pthread_mutex_lock(&data.server_dead_philo);
				if (data.dead_philo != -1)
					data.run = 0;
				pthread_mutex_unlock(&data.server_dead_philo);
				usleep(5);
				continue ;
			}
			data.request_pending = data.request;
			data.request = -1;
			safe_print(data.request_pending, "Monitor has a request\n", &data.print, 1);
			pthread_mutex_unlock(&data.server_request);
		}
		pthread_mutex_lock(&data.server_answer);
		ok +=1;
		data.answer = ok % 3;
		if (data.answer)
			safe_print(data.request_pending, "Monitor gives OK to request\n", &data.print, 1);
		else
			safe_print(data.request_pending, "Monitor gives KO to request\n", &data.print, 1);
		pthread_mutex_unlock(&data.server_answer);
		pthread_mutex_lock(&data.server_dead_philo);
		if (data.dead_philo != -1)
			data.run = 0;
		pthread_mutex_unlock(&data.server_dead_philo);
	}
	safe_print(data.dead_philo, "Monitor has received RIP status\n", &data.print, 1);
	i = 0;
	while (i < 3)
	{
		pthread_join(data.thread[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&data.fork1);
	pthread_mutex_destroy(&data.fork2);
	pthread_mutex_destroy(&data.fork3);
	pthread_mutex_destroy(&data.server_request);
	pthread_mutex_destroy(&data.server_answer);
	pthread_mutex_destroy(&data.server_com);
	pthread_mutex_destroy(&data.server_dead_philo);
	pthread_mutex_destroy(&data.print);
	return (1);
}
