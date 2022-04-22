/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:17:42 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/22 17:30:35 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

int	g_continue = 1;

void	signal_handler(int signal)
{
	(void)signal;
	g_continue = 0;
}

void	safe_print(int id, char *txt, pthread_mutex_t *print_mutex, int monitor)
{
	pthread_mutex_lock(print_mutex);
	if (monitor)
		printf("\x1b[33m");
	if (id >= 0)
		printf("Philo %d ", id);
	printf("%s", txt);
	if (monitor)
		printf("\x1b[0m");
	pthread_mutex_unlock(print_mutex);
}

void	*philo_routine(void *phil)
{
	t_philo	*philo;
	philo = (t_philo *)phil;
	while (g_continue)
	{
		pthread_mutex_lock(&philo->data->server_com);
		safe_print(philo->id, "Com ok\n", &philo->data->print, 0);
		pthread_mutex_lock(&philo->data->server_request);
		philo->data->request = philo->id;
		safe_print(philo->id, "Request made\n", &philo->data->print, 0);
		pthread_mutex_unlock(&philo->data->server_request);
		while (philo->answer == -1)
		{
			pthread_mutex_lock(&philo->data->server_answer);
			if (philo->data->answer == -1)
			{
				safe_print(philo->id, "Waiting for answer\n", &philo->data->print, 0);
				pthread_mutex_unlock(&philo->data->server_answer);
				usleep(20);
				continue ;
			}
			safe_print(philo->id, "Answer is given\n", &philo->data->print, 0);
			philo->answer = philo->data->answer;
			philo->data->answer = -1;
			pthread_mutex_unlock(&philo->data->server_answer);
		}
		pthread_mutex_unlock(&philo->data->server_com);
		safe_print(philo->id, "Releasing com\n", &philo->data->print, 0);
		if (philo->answer == 0)
			safe_print(philo->id, "answer is KO\n", &philo->data->print, 0);
		else
			safe_print(philo->id, "answer is OK\n", &philo->data->print, 0);
		usleep(200);
	}
	return (phil);
}

int	main(void)
{
	long		i;
	static int	ok = 0;
	t_data	data;
	t_philo	philo[3];
	signal(SIGINT, signal_handler);

	pthread_mutex_init(&data.fork1, NULL);
	pthread_mutex_init(&data.fork2, NULL);
	pthread_mutex_init(&data.fork3, NULL);
	pthread_mutex_init(&data.server_request, NULL);
	pthread_mutex_init(&data.server_answer, NULL);
	pthread_mutex_init(&data.server_com, NULL);
	pthread_mutex_init(&data.print, NULL);
	data.request = -1;
	data.request_pending = -1;
	data.answer = -1;

	i = 0;
	while (i < 3)
	{
		philo[i].id = i;
		philo[i].answer = -1;
		philo[i].data = &data;
		pthread_create(&data.thread[i], NULL, philo_routine, &philo[i]);
		i++;
	}
	while (g_continue)
	{
		data.request_pending = -1;
		while (data.request_pending == -1)
		{
			pthread_mutex_lock(&data.server_request);
			if (data.request == -1)
			{
				safe_print(-1, "Monitor waiting for request\n", &data.print, 1);
				pthread_mutex_unlock(&data.server_request);
				usleep(20);
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
	}
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
	pthread_mutex_destroy(&data.print);
	return (1);
}
