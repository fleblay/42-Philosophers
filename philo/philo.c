/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:17:42 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/22 15:18:35 by fle-blay         ###   ########.fr       */
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

void	*printer(void *phil)
{
	t_philo	*philo;
	philo = (t_philo *)phil;
	while (g_continue)
	{
		pthread_mutex_lock(&philo->data->server_request);
		pthread_mutex_lock(&philo->data->print);
		printf("Philo %d has made a request to eat\n", philo->id);
		fflush(stdout);
		pthread_mutex_unlock(&philo->data->print);
		philo->data->request = philo->id;
		philo->request = 1;
		pthread_mutex_unlock(&philo->data->server_request);
		pthread_mutex_lock(&philo->data->server_answer);
		if (philo->data->answer == 1)
		{
			philo->request = -1;
			philo->data->answer = 0;
			pthread_mutex_unlock(&philo->data->server_answer);
			pthread_mutex_lock(&philo->data->print);
			printf("Philo %d starts eating\n", philo->id);
			fflush(stdout);
			pthread_mutex_unlock(&philo->data->print);
			usleep(200);
			pthread_mutex_lock(&philo->data->print);
			printf("Philo %d has finished eating\n", philo->id);
			fflush(stdout);
			pthread_mutex_unlock(&philo->data->print);
		}
		else
		{
			pthread_mutex_unlock(&philo->data->server_answer);
			usleep(10);
		}
	}
	return (phil);
}

int	main(void)
{
	long		i;
	t_data	data;
	t_philo	philo[3];
	signal(SIGINT, signal_handler);

	pthread_mutex_init(&data.fork1, NULL);
	pthread_mutex_init(&data.fork2, NULL);
	pthread_mutex_init(&data.fork3, NULL);
	pthread_mutex_init(&data.server_request, NULL);
	pthread_mutex_init(&data.server_answer, NULL);
	pthread_mutex_init(&data.print, NULL);
	data.request = -1;
	data.answer = 0;

	i = 0;
	while (i < 3)
	{
		philo[i].id = i;
		philo[i].request = 0;
		philo[i].data = &data;
		pthread_create(&data.thread[i], NULL, printer, &philo[i]);
		i++;
	}
	while (g_continue)
	{
		pthread_mutex_lock(&data.server_request);
		pthread_mutex_lock(&data.print);
		printf("request from %d\n", data.request);
		fflush(stdout);
		pthread_mutex_unlock(&data.print);
		data.request = 0;
		pthread_mutex_lock(&data.server_answer);
		pthread_mutex_lock(&data.print);
		printf("setting answer to 1\n");
		fflush(stdout);
		pthread_mutex_unlock(&data.print);
		data.answer = 1;	
		pthread_mutex_unlock(&data.server_answer);
		pthread_mutex_unlock(&data.server_request);
		usleep(10);
	}
	pthread_mutex_lock(&data.print);
	printf("Leaving\n");
	fflush(stdout);
	pthread_mutex_unlock(&data.print);
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
	return (1);
}
