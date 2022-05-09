/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:29:05 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/09 16:50:15 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <stdlib.h>
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
	printf("dead est mis a 1 pour %d\n", data->id);
	sem_post(data->s_self_dead[data->id]);
	sem_post(data->s_ack_msg);
	return (NULL);
}

//keeping res for testing purposes
void	ft_create_philo_monitor(t_data *data, pthread_t *death_monitor)
{
	int	res;

	/*
	if (data->id == 2)
		res = 1;
	else
	*/
	res = pthread_create(death_monitor, NULL, ft_end_simulation, data); 
	if (res)
	{
		sem_post(data->s_dead_signal);
		//simulating ack for dead philo who returned
		sem_post(data->s_ack_msg);
		sem_wait(data->s_end_of_termination);
		sem_post(data->s_end_of_termination);
		sem_post(data->s_philo_deamon);
		ft_sem_destroy(data, ALL);
		ft_deallocate(data);
		ft_putstr_fd("Error : thread create failure in philo\n", 2);
		exit (1);
	}
}

int	ft_philo_routine(t_data *data, int i)
{
	pthread_t	death_monitor;
	int			index;

	data->id = i;
	index = 0;
	ft_create_philo_monitor(data, &death_monitor);
	printf("Deamon is up and running %d\n", data->id);
	sem_post(data->s_philo_deamon);
	sem_wait(data->s_start);
	printf("just before check self is dead %d\n", i);
	check_if_end_sim(data);
	printf("data->go_on : %d from  %d\n",data->go_on, i);
	while (data->go_on)
	{
		// test satiated philo
		if (index == 4)
		{
			sem_post(data->s_meal);
		}
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
	printf("Leaving now %d\n", data->id);
	return (0);
}
