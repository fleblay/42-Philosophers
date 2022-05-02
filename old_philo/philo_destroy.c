/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_destroy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 14:05:07 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/29 16:39:17 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

void	cleanup(t_data *data)
{
	int i;

	i = 0;
	DEBUG && safe_print_monitor("DEBUG && Start joining threads\n", data);
	while (i < data->philo_count)
	{
		pthread_join(data->thread[i], NULL);
		i++;
	}
	DEBUG && safe_print_monitor("DEBUG && Finished waiting threads\n", data);
	i = 0;
	DEBUG && safe_print_monitor("DEBUG && Start destroying mutexes forks\n", data);
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	DEBUG && safe_print_monitor("DEBUG && Finished destroying mutexes forks\n", data);
	print_meal_count(&data->philo[0]);
	free(data->thread);
	free(data->fork);
	free(data->fork_available);
	free(data->meal_count);
	free(data->philo);
	pthread_mutex_destroy(&data->server_request);
	pthread_mutex_destroy(&data->server_answer);
	pthread_mutex_destroy(&data->server_com);
	pthread_mutex_destroy(&data->server_available_com);
	pthread_mutex_destroy(&data->server_dead_philo);
	pthread_mutex_destroy(&data->meal);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->start);
	pthread_mutex_destroy(&data->time);
	//printf("Fin Simu\n");
}
