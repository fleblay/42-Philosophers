/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:29:05 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/10 11:56:42 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int		ft_self_is_dead(t_data *data)
{
	data->current_time = ft_get_time() - data->start_time;
	if (data->current_time - data->last_start_eat > data->ttd)
	{
		sem_post(data->s_dead_signal);
		ft_safe_print(data, "died", 1);
		return (1);
	}
	return (0);
}

int	ft_grab_forks(t_data *data)
{
	//sem_wait(data->s_start);
	printf("locked waiting fork1 : %d\n", data->id + 1);
	sem_wait(data->s_fork);
	ft_safe_print(data, "has taken a fork", 0);
	printf("locked waiting fork2 : %d\n", data->id + 1);
	sem_wait(data->s_fork);
	ft_safe_print(data, "has taken a fork", 0);
	//sem_post(data->s_start);
	return (1);
}

int	ft_eat(t_data *data)
{
		check_if_end_sim(data);
		if (data->go_on == 0 || ft_self_is_dead(data))
		{
			printf("givin back 2 forks %d\n", data->id + 1);
			sem_post(data->s_fork);
			sem_post(data->s_fork);
			return (0);
		}
	data->current_time = ft_get_time() - data->start_time;
	data->last_start_eat = data->current_time;
	ft_safe_print(data, "is eating", 0);
	while (data->current_time - data->last_start_eat < data->tte)
	{
		check_if_end_sim(data);
		if (data->go_on == 0 || ft_self_is_dead(data))
		{
			printf("givin back 2 forks %d\n", data->id + 1);
			sem_post(data->s_fork);
			sem_post(data->s_fork);
			return (0);
		}
		else
			usleep(500);
		data->current_time = ft_get_time() - data->start_time;
	}
	printf("givin back 2 forks %d\n", data->id + 1);
	sem_post(data->s_fork);
	sem_post(data->s_fork);
	data->meal_goal--;
	if (!data->meal_goal)
		sem_post(data->s_meal);
	return (1);
}

int	ft_sleep(t_data *data)
{
	data->current_time = ft_get_time() - data->start_time;
	data->last_start_sleep = data->current_time;
	ft_safe_print(data, "is sleeping", 0);
	while (data->current_time - data->last_start_sleep < data->tts)
	{
		check_if_end_sim(data);
		if (data->go_on == 0 || ft_self_is_dead(data))
			return (0);
		else
			usleep(500);
		data->current_time = ft_get_time() - data->start_time;
	}
	return (1);
}

int	ft_think(t_data *data)
{
	data->current_time = ft_get_time() - data->start_time;
	data->last_start_think = data->current_time;
	ft_safe_print(data, "is thinking", 0);
	if (data->ttd < data->tte + data->tts + 10)
		return (1);
	while (data->current_time - data->last_start_think
		< data->ttd - data->tte - data->tts - 10)
	{
		check_if_end_sim(data);
		if (data->go_on == 0 || ft_self_is_dead(data))
			return (0);
		else
			usleep(500);
		data->current_time = ft_get_time() - data->start_time;
	}
	return (1);
}

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
	//printf("dead est mis a 1 pour %d\n", data->id);
	sem_post(data->s_self_dead[data->id]);
	sem_post(data->s_ack_msg);
	return (NULL);
}

//keeping res for testing purposes
void	ft_create_philo_monitor(t_data *data, pthread_t *death_monitor)
{
	int	res;

	/*
	if (data->id == 2 || data->id == 3)
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
		//sem_post(data->s_end_of_termination);
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

	data->id = i;
	ft_create_philo_monitor(data, &death_monitor);
	printf("Deamon is up and running %d\n", data->id);
	sem_post(data->s_philo_deamon);
	sem_wait(data->s_start);
	data->start_time = ft_get_time();
	data->last_start_eat = 0;
	printf("just before check self is dead %d\n", i);
	check_if_end_sim(data);
	printf("data->go_on : %d from  %d\n",data->go_on, i);
	if ((data->id + 1)% 2 == 0)
		usleep((data->tte - 10) * 1000);
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
		if (index == 4)
		{
			sem_post(data->s_dead_signal);
			ft_safe_print(data, "died", 1);
			break ;
		}
		*/
		// test dead philo
		if (!ft_grab_forks(data))
			break ;
		if (!ft_eat(data))
			break ;
		if (!ft_sleep(data))
			break ;
		if (!ft_think(data))
			break ;
		check_if_end_sim(data);
	}
	pthread_join(death_monitor, NULL);
	ft_sem_destroy(data, ALL);
	ft_deallocate(data);
	//printf("Leaving now %d\n", data->id);
	return (0);
}
