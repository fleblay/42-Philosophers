/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:29:05 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/11 15:10:34 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int	ft_sim_is_over(t_data *data)
{
	sem_wait(data->s_self_dead[data->id]);
	if (data->dead == 1)
	{
		sem_post(data->s_self_dead[data->id]);
		data->go_on = 0;
		return (1);
	}
	sem_post(data->s_self_dead[data->id]);
	return (0);
}

void	*ft_eos_checker(void *param)
{
	t_data		*data;

	data = (t_data *)param;
	sem_wait(data->s_end_simu);
	sem_wait(data->s_self_dead[data->id]);
	data->dead = 1;
	printf("dead est mis a 1 pour %d\n", data->id);
	sem_post(data->s_self_dead[data->id]);
	sem_post(data->s_ack_msg);
	printf("eos returning\n");
	return (NULL);
}

void	*ft_dead_checker(void *param)
{
	t_data		*data;

	data = (t_data *)param;
	while (42)
	{
		if (ft_self_is_dead(data))
			break ;
		usleep(500);
	}
	printf("dead checker returning\n");
	return (NULL);
}

//keeping res for testing purposes
void	ft_create_philo_monitor(t_data *data, pthread_t *eos_monitor, pthread_t *am_i_dead_monitor)
{
	int	res;
	/*
	if (data->id == 2 || data->id == 3)
		res = 1;
	else
	*/
		res = pthread_create(am_i_dead_monitor, NULL, ft_dead_checker, data); 
	if (res)
	{
		ft_fake_own_death(data);
		ft_cleanup_exit_error(data, "Error : thread create failure in philo\n");
	}
	/*
	if (data->id == 2 || data->id == 3)
		res = 1;
	else
	*/
		res = pthread_create(eos_monitor, NULL, ft_eos_checker, data); 
	if (res)
	{
		ft_fake_own_death(data);
		ft_kill_philo_am_i_dead_monitor(data);
		pthread_join(*am_i_dead_monitor, NULL);
		ft_cleanup_exit_error(data, "Error : thread create failure in philo\n");
	}
}

int	ft_philo_routine(t_data *data, int i)
{
	pthread_t	eos_monitor;
	pthread_t	am_i_dead_monitor;

	data->id = i;
	ft_create_philo_monitor(data, &eos_monitor, &am_i_dead_monitor);
	//printf("Deamon is up and running %d\n", data->id);
	sem_post(data->s_philo_deamon);
	sem_wait(data->s_start);
	sem_wait(data->s_self_dead[data->id]);
	data->start_time = ft_get_time();
	sem_post(data->s_self_dead[data->id]);
	//printf("just before check self is dead %d\n", i);
	//printf("data->go_on : %d from  %d\n",data->go_on, i);
	if ((data->id + 1) % 2 == 0)
		usleep((data->tte - 10) * 1000);
	while (data->go_on)
	{
		if (ft_sim_is_over(data))
			break ;
		ft_grab_forks(data);
		if (!ft_eat(data) || !ft_sleep(data) || !ft_think(data))
			break ;
	}
	pthread_join(eos_monitor, NULL);
	pthread_join(am_i_dead_monitor, NULL);
	ft_sem_destroy(data, ALL);
	ft_deallocate(data);
	//printf("\x1b[32mLeaving now %d\n\x1b[0m", data->id + 1);
	return (0);
}
