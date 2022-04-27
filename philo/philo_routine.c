/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 15:41:24 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/27 12:44:19 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

void	wait_for_com_token(t_philo *philo)
{
	while (philo->dead == -1 && philo->has_com == -1)
	{
		if (self_is_dead(philo) || someone_is_dead(philo))
			break ;
		pthread_mutex_lock(&philo->data->server_available_com);
		if (philo->data->available_com == -1)
		{
			safe_print(philo->id, "Waiting for available com\n", &philo->data->print, 0);
			pthread_mutex_unlock(&philo->data->server_available_com);
			usleep(200);
			continue ;
		}
		safe_print(philo->id, "grabing available com token\n", &philo->data->print, 0);
		philo->has_com = 1;
		philo->data->available_com = -1;
		pthread_mutex_unlock(&philo->data->server_available_com);
	}
	return ;
}

void	grab_com_and_place_request(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->server_com);
	safe_print(philo->id, "Com ok\n", &philo->data->print, 0);
	pthread_mutex_lock(&philo->data->server_request);
	philo->data->request = philo->id;
	safe_print(philo->id, "Request made\n", &philo->data->print, 0);
	pthread_mutex_unlock(&philo->data->server_request);
	philo->answer = -1;
	return ;
}

void	wait_for_answer(t_philo *philo)
{
	while (philo->dead == -1 && philo->answer == -1)
	{
		if (self_is_dead(philo) || someone_is_dead(philo))
			break ;
		pthread_mutex_lock(&philo->data->server_answer);
		if (philo->data->answer == -1)
		{
			safe_print(philo->id, "Waiting for answer\n", &philo->data->print, 0);
			pthread_mutex_unlock(&philo->data->server_answer);
			usleep(80);
			continue ;
		}
		safe_print(philo->id, "Answer is given after waiting\n", &philo->data->print, 0);
		philo->answer = philo->data->answer;
		philo->data->answer = -1;
		pthread_mutex_unlock(&philo->data->server_answer);
	}
	safe_print(philo->id, "Answer is given\n", &philo->data->print, 0);
	return ;
}

void	release_com_token_and_com(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->server_com);
	safe_print(philo->id, "Releasing com\n", &philo->data->print, 0);
	pthread_mutex_lock(&philo->data->server_available_com);
	safe_print(philo->id, "Releasing available com token\n", &philo->data->print, 0);
	philo->has_com = -1;
	philo->data->available_com = 1;
	pthread_mutex_unlock(&philo->data->server_available_com);
	return ;
}

void	eat_for_time(t_philo *philo)
{
	if (self_is_dead(philo) || someone_is_dead(philo))
		return ;
	philo->start_eat = get_sim_duration();
	safe_print(philo->id, "Start eating\n", &philo->data->print, 0);
	while (philo->dead == -1 && get_sim_duration() - philo->start_eat < philo->data->tte)
	{
		if (self_is_dead(philo) || someone_is_dead(philo))
			break ;
		usleep(1000);
	}
	if (philo->dead == -1)
	{
		safe_print(philo->id, "Finished eating\n", &philo->data->print, 0);
		//philo->time_last_eat = get_sim_duration();
		safe_print(philo->id, "Updating meal count\n", &philo->data->print, 0);
		pthread_mutex_lock(&philo->data->meal);
		philo->data->meal_count[philo->id - 1]++;
		pthread_mutex_unlock(&philo->data->meal);
	}
	else
		safe_print(philo->id, "Someone died while I was eating\n", &philo->data->print, 0);
	return ;
}

void	sleep_for_time(t_philo *philo)
{
	if (self_is_dead(philo) || someone_is_dead(philo))
		return ;
	philo->start_sleep = get_sim_duration();
	safe_print(philo->id, "Start sleeping\n", &philo->data->print, 0);
	while (philo->dead == -1 && get_sim_duration() - philo->start_sleep < philo->data->tts)
	{
		if (self_is_dead(philo) || someone_is_dead(philo))
			break ;
		usleep(1000);
	}
	if (philo->dead == -1)
	{
		safe_print(philo->id, "Finished sleeping\n", &philo->data->print, 0);
	}
	else
		safe_print(philo->id, "Someone died while I was sleeping\n", &philo->data->print, 0);
	return ;
}

void	lock_forks(t_philo *philo)
{
	safe_print(philo->id, "Picking up forks\n", &philo->data->print, 0);
	pthread_mutex_lock(&philo->data->fork[philo->index_fork1]);
	philo->data->fork_available[philo->index_fork1] = 0;
	pthread_mutex_unlock(&philo->data->fork[philo->index_fork1]);
	pthread_mutex_lock(&philo->data->fork[philo->index_fork2]);
	philo->data->fork_available[philo->index_fork2] = 0;
	pthread_mutex_unlock(&philo->data->fork[philo->index_fork2]);
}

void	release_forks(t_philo *philo)
{
	safe_print(philo->id, "Releasing forks\n", &philo->data->print, 0);
	pthread_mutex_lock(&philo->data->fork[philo->index_fork1]);
	philo->data->fork_available[philo->index_fork1] = 1;
	pthread_mutex_unlock(&philo->data->fork[philo->index_fork1]);
	pthread_mutex_lock(&philo->data->fork[philo->index_fork2]);
	philo->data->fork_available[philo->index_fork2] = 1;
	pthread_mutex_unlock(&philo->data->fork[philo->index_fork2]);
}
