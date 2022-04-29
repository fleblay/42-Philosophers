/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:31:08 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/29 10:36:33 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

int	monitor_someone_is_dead(t_data *data)
{
	pthread_mutex_lock(&data->server_dead_philo);
	if (data->dead_philo != -1)
	{
		pthread_mutex_unlock(&data->server_dead_philo);
		data->philo_is_dead = 1;
		DEBUG && safe_print_monitor("DEBUG Found out dead philo\n", data);
		return (1);
	}
	pthread_mutex_unlock(&data->server_dead_philo);
	return (0);
}
