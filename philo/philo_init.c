/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 12:23:15 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/28 18:44:35 by fle-blay         ###   ########.fr       */
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
	pthread_mutex_init(&data->meal, NULL);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->start, NULL);
	pthread_mutex_init(&data->time_mutex, NULL);
	//mutex init to protect;
	data->request_pending = -1;
	data->request = -1;
	data->answer = -1;
	data->dead_philo = -1;
	data->run = 1;
	data->available_com = 1;
	data->meal_goal_achieved = 0;
	data->philo_is_dead = 0;
	data->philo_count = atoi(av[1]);
	data->ttd = atoi(av[2]);
	data->tte = atoi(av[3]);
	data->tts = atoi(av[4]);
	data->ttt = data->ttd - data->tte - data->tts - 5;
	data->ttt = (data->ttt > 0) * data->ttt;
	if (ac > 5)
		data->ntepme = atoi(av[5]);
	else
		data->ntepme = -1;
	data->thread = malloc(data->philo_count * sizeof(pthread_t));
	data->fork = malloc(data->philo_count * sizeof(pthread_mutex_t));
	data->fork_available = malloc(data->philo_count * sizeof(int));
	data->meal_count = malloc(data->philo_count * sizeof(int));
	data->philo = malloc(data->philo_count * sizeof(t_philo));
	return ((data->thread != 0) * (data->fork != 0) * (data->philo!=0) * (data->fork_available != 0) * (data->meal_count != 0));
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
		data->philo[i].meal_goal_achieved = 0;
		data->philo[i].start_sleep = 0;
		data->philo[i].start_eat = 0;
		pthread_mutex_init(&data->fork[i], NULL);
		data->fork_available[i] = 1;
		data->meal_count[i] = 0;
		data->philo[i].index_fork1 = data->philo[i].id - 1;
		data->philo[i].index_fork2 = (data->philo[i].id) % data->philo_count;
		data->philo[i].ttt = data->ttt;
		data->philo[i].ttd = data->ttd;
		data->philo[i].tte = data->tte;
		data->philo[i].tts = data->tts;
		data->philo[i].current_time = 0;	
		data->philo[i].philo_count = data->philo_count;
		//printf("Philo id [%d] will try to pick up forks [%d] and [%d]\n", i+1, data->philo[i].index_fork1, data->philo[i].index_fork2);
		//mutex init to protect;
		i++;
	}
	return (1);
}

int	launch_philo(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->start);
	while (i < data->philo_count)
	{
		pthread_create(&data->thread[i], NULL, philo_routine, &data->philo[i]);
		//thread init to protect;
		i+=2;
	}
	i = 1;
	while (i < data->philo_count)
	{
		pthread_create(&data->thread[i], NULL, philo_routine, &data->philo[i]);
		//thread init to protect;
		i+=2;
	}
	pthread_mutex_lock(&data->time_mutex);
	get_sim_duration();
	pthread_mutex_unlock(&data->time_mutex);
	pthread_mutex_unlock(&data->start);
	return (1);
}
