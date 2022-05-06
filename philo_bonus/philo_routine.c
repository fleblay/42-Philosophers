/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:29:05 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/06 13:13:14 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void	check_if_end_sim(t_data *data)
{
	sem_wait(data->s_self_dead[data->id]);
	if (data->dead == 1)
		data->go_on = 0;
	sem_post(data->s_self_dead[data->id]);
}

void	*ft_end_simulation(void *param)
{
	t_data		*data;

	data = (t_data *)param;
	sem_wait(data->s_end_simu);
	sem_wait(data->s_self_dead[data->id]);
	data->dead = 1;
	sem_post(data->s_self_dead[data->id]);
	sem_post(data->s_ack_msg);
	return (NULL);
}

int	ft_philo_routine(t_data *data, int i)
{
	pthread_t	death_monitor;
	int			index;

	data->id = i;
	index = 0;
	// Ajout d'un check sur le fail des init create
	printf("from child %d\n", i);
	pthread_create(&death_monitor, NULL, ft_end_simulation, data);
	sem_post(data->s_philo_deamon);
	sem_wait(data->s_start);
	printf("from before loop %d\n", i);
	check_if_end_sim(data);
	while (data->go_on)
	{
		// test satiated philo
		/*
		if (index == 4)
		{
			sem_post(data->s_meal);
		}
		*/
		// test satiated philo

		// test dead philo
		/*
		if (i == 2 && index == 2)
		{
			sem_post(data->s_dead_signal);
		}
		*/
		// test dead philo
		usleep(1000000);
		printf("loop from child %d\n", i);
		check_if_end_sim(data);
		index++;
	}
	pthread_join(death_monitor, NULL);
	ft_sem_destroy(data, ALL);
	ft_deallocate(data);
	return (0);
}
