/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 12:23:15 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/26 18:40:08 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

int	init_data(t_data *data, int ac, char *av[])
{
	pthread_mutex_init(&data->server_request, NULL);
	pthread_mutex_init(&data->server_answer, NULL);
	pthread_mutex_init(&data->server_com, NULL);
	pthread_mutex_init(&data->server_available_com, NULL);
	pthread_mutex_init(&data->server_dead_philo, NULL);
	pthread_mutex_init(&data->print, NULL);
	//mutex init to protect;
	data->request_pending = -1;
	data->request = -1;
	data->answer = -1;
	data->dead_philo = -1;
	data->run = 1;
	data->available_com = 1;
	data->philo_count = atoi(av[1]);
	data->ttd = atoi(av[2]);
	data->tte = atoi(av[3]);
	data->tts = atoi(av[4]);
	if (ac > 5)
		data->ntepme = atoi(av[4]);
	else
		data->ntepme = -1;
	data->thread = malloc(data->philo_count * sizeof(pthread_t));
	data->fork = malloc(data->philo_count * sizeof(pthread_mutex_t));
	data->fork_available = malloc(data->philo_count * sizeof(int));
	data->philo = malloc(data->philo_count * sizeof(t_philo));
	return ((data->thread != 0) * (data->fork != 0) * (data->philo!=0) * (data->fork_available != 0));
}

int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philo[i].data = data;
		data->philo[i].id = i + 1;
		data->philo[i].has_com = -1;
		data->philo[i].answer = -1;
		data->philo[i].dead = -1;
		data->philo[i].time_last_eat = 0;
		data->philo[i].start_sleep = -1;
		data->philo[i].start_eat = -1;
		pthread_mutex_init(&data->fork[i], NULL);
		data->fork_available[i] = 1;
		data->philo[i].index_fork1 = data->philo[i].id - 1;
		data->philo[i].index_fork2 = data->philo[i].id - 1 % data->philo_count;
		//mutex init to protect;
		i++;
	}
	return (1);
}

int	launch_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_create(&data->thread[i], NULL, philo_routine, &data->philo[i]);
		//thread init to protect;
		i++;
	}
	return (1);
}
