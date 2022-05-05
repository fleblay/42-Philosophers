/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:29:05 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/05 16:34:44 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void	*ft_death_monitor(void *param)
{
	t_data		*data;

	data = (t_data *)param;
	sem_wait(data->s_dead);
	data->dead = 1;
	sem_post(data->s_dead);
	return (NULL);
}

int	ft_philo_routine(t_data *data, int i)
{
	pthread_t	death_monitor;
	int			index;

	index = 0;
	sem_wait(data->s_start);
	printf("from child %d\n", i);
	sem_post(data->s_start);
	pthread_create(&death_monitor, NULL, ft_death_monitor, data);
	pthread_detach(death_monitor);
	while (!data->dead)
	{
		if (index == 4)
		{
			sem_post(data->s_meal);
		}

		// test dead philo
		/*
		if (i == 2 && index == 2)
		{
			sem_post(data->s_dead);
		}
		*/
		// test dead philo
		usleep(1000000);
		printf("loop from child %d\n", i);
		index++;
	}
	ft_sem_destroy(data, ALL);
	ft_deallocate(data);
	return (0);
}
