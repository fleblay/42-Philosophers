/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:29:05 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/09 12:54:33 by fred             ###   ########.fr       */
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
	// pour empecher debut boucle data->go_on en cas de thead_init failure d'un autre
	// on bloque le check d'avant le debut de boucle en ayant le sem a 0 avant de
	// rentrer dans cette fonction
	//to fix
	sem_wait(data->s_end_simu);
	sem_wait(data->s_self_dead[data->id]);
	data->dead = 1;
	printf("dead est mis a 1 pour %d\n", data->id);
	sem_post(data->s_self_dead[data->id]);
	sem_post(data->s_ack_msg);
	return (NULL);
}

int	ft_philo_routine(t_data *data, int i)
{
	pthread_t	death_monitor;
	int			index;
	int			res;

	data->id = i;
	index = 0;
	// Ajout d'un check sur le fail des init create
	///*
	printf("from child %d\n", i);
	if (data->id == 2)
		res = 1;
	else
	//*/
		res = pthread_create(&death_monitor, NULL, ft_end_simulation, data); 
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
		return (ft_putstr_fd("Error : thread create failure in philo\n", 2), 1);
	}
	printf("Deamon is up and running %d\n", data->id);
	sem_post(data->s_philo_deamon);
	sem_wait(data->s_start);
	printf("just before check self is dead %d\n", i);
	check_if_end_sim(data);
	printf("data->go_on : %d from  %d\n",data->go_on, i);
	while (data->go_on)
	{
		// test satiated philo
		///*
		if (index == 4)
		{
			sem_post(data->s_meal);
		}
		//*/
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
