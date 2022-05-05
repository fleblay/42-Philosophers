/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:29:05 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/05 17:53:11 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void	*ft_self_death_monitor(void *param)
{
	t_data		*data;

	data = (t_data *)param;
	sem_wait(data->s_dead);
	data->dead = 1;
	//to kill meal deamon
	sem_post(data->s_meal);
	return (NULL);
}

int	ft_philo_routine(t_data *data, int i)
{
	pthread_t	death_monitor;
	int			index;

	index = 0;
	pthread_create(&death_monitor, NULL, ft_self_death_monitor, data);
	pthread_detach(death_monitor);
	sem_wait(data->s_start);
	printf("from child %d\n", i);
	while (!data->dead)
	{
		/*
		if (index == 4)
		{
			sem_post(data->s_meal);
		}
		*/

		// test dead philo
		if (i == 2 && index == 2)
		{
			sem_post(data->s_dead_signal);
		}
		
		// test dead philo
		usleep(1000000);
		printf("loop from child %d\n", i);
		index++;
	}
	ft_sem_destroy(data, ALL);
	ft_deallocate(data);
	return (0);
}
