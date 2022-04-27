/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_destroy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 14:05:07 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/27 12:44:38 by fle-blay         ###   ########.fr       */
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
	safe_print(0 , "Start joining threads\n", &data->print, 1);
	while (i < data->philo_count)
	{
		pthread_join(data->thread[i], NULL);
		i++;
	}
	safe_print(0 , "Finished waiting threads\n", &data->print, 1);
	i = 0;
	safe_print(0 , "Start destroying mutexes forks\n", &data->print, 1);
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	safe_print(0 , "Finished destroying mutexes forks\n", &data->print, 1);
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
	printf("Fin Simu\n");
}
