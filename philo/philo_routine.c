/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 15:41:24 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/29 12:33:40 by fle-blay         ###   ########.fr       */
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
		/*
		if (self_is_dead(philo) || someone_is_dead(philo))
			break ;
		*/
		pthread_mutex_lock(&philo->data->server_available_com);
		if (philo->data->available_com == -1)
		{
			DEBUG && safe_print("DEBUG && Waiting for available com\n", philo);
			pthread_mutex_unlock(&philo->data->server_available_com);
			usleep(50);
			continue ;
		}
		DEBUG && safe_print("DEBUG && grabing available com token\n", philo);
		philo->has_com = 1;
		philo->data->available_com = -1;
		pthread_mutex_unlock(&philo->data->server_available_com);
	}
	return ;
}

void	grab_com_and_place_request(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->server_com);
	DEBUG && safe_print("DEBUG && Com ok\n", philo);
	pthread_mutex_lock(&philo->data->server_request);
	philo->data->request = philo->id;
	DEBUG && safe_print("DEBUG && Request made\n", philo);
	pthread_mutex_unlock(&philo->data->server_request);
	philo->answer = -1;
	return ;
}

void	wait_for_answer(t_philo *philo)
{
	while (philo->dead == -1 && philo->answer == -1)
	{
		/*
		if (self_is_dead(philo) || someone_is_dead(philo))
			break ;
		*/
		pthread_mutex_lock(&philo->data->server_answer);
		if (philo->data->answer == -1)
		{
			DEBUG && safe_print("DEBUG && Waiting for answer\n", philo);
			pthread_mutex_unlock(&philo->data->server_answer);
			usleep(50);
			continue ;
		}
		DEBUG && safe_print("DEBUG && Answer is given after waiting\n", philo);
		philo->answer = philo->data->answer;
		// On reset la reponse uniquement si elle vaut OK ou KO
		if (philo->data->answer >= 0)
			philo->data->answer = -1;
		pthread_mutex_unlock(&philo->data->server_answer);
	}
	DEBUG && safe_print("DEBUG && Answer is given\n", philo);
	return ;
}

void	release_com_token_and_com(t_philo *philo)
{
	DEBUG && safe_print("DEBUG && Releasing com\n", philo);
	pthread_mutex_unlock(&philo->data->server_com);
	pthread_mutex_lock(&philo->data->server_available_com);
	DEBUG && safe_print("DEBUG && Releasing com token\n", philo);
	philo->data->available_com = 1;
	pthread_mutex_unlock(&philo->data->server_available_com);
	philo->has_com = -1;
	return ;
}

void	eat_for_time(t_philo *philo)
{
	/*
	if (self_is_dead(philo) || someone_is_dead(philo))
		return ;
	*/
	pthread_mutex_lock(&philo->data->time_mutex);
	philo->start_eat = get_time() - philo->start_time;
	philo->current_time = philo->start_eat;
	pthread_mutex_unlock(&philo->data->time_mutex);
	safe_print("is eating\n", philo);
	while (philo->dead == -1 && philo->current_time - philo->start_eat < philo->tte)
	{
		/*
		if (self_is_dead(philo) || someone_is_dead(philo))
			break ;
		*/
		usleep(1000);
		pthread_mutex_lock(&philo->data->time_mutex);
		philo->current_time = get_time() - philo->start_time;
		pthread_mutex_unlock(&philo->data->time_mutex);
	}
	if (philo->dead == -1)
	{
		DEBUG && safe_print("DEBUG && Finished eating\n", philo);
		pthread_mutex_lock(&philo->data->meal);
		DEBUG && safe_print("DEBUG && Updating meal count\n", philo);
		philo->data->meal_count[philo->id - 1]++;
		pthread_mutex_unlock(&philo->data->meal);
		//print_meal_count(philo);
	}
	else
	{
		DEBUG && safe_print("DEBUG && Someone died while I was eating\n", philo);
	}
	return ;
}

void	sleep_for_time(t_philo *philo)
{
	/*
	if (self_is_dead(philo) || someone_is_dead(philo))
		return ;
	*/
	pthread_mutex_lock(&philo->data->time_mutex);
	philo->start_sleep = get_time() - philo->start_time;
	philo->current_time = philo->start_sleep;
	pthread_mutex_unlock(&philo->data->time_mutex);
	safe_print("is sleeping\n", philo);
	// utiliser philo_current time
	while (philo->dead == -1 && philo->current_time - philo->start_sleep < philo->tts)
	{
		/*
		if (self_is_dead(philo) || someone_is_dead(philo))
			break ;
		*/
		usleep(1000);
		pthread_mutex_lock(&philo->data->time_mutex);
		philo->current_time = get_time() - philo->start_time;
		pthread_mutex_unlock(&philo->data->time_mutex);
	}
	if (philo->dead == -1)
	{
		safe_print("is thinking\n", philo);
		if (philo->ttt > 0 && philo->philo_count % 2)
		{
			DEBUG && safe_print("DEBUG && Start Waiting for synchro\n", philo);
			usleep(philo->ttt * 1000);
			DEBUG && safe_print("DEBUG && Finished Waiting for synchro\n", philo);
		}
	}
	else
	{
		DEBUG && safe_print("DEBUG && Someone died while I was sleeping\n", philo);
	}

	return ;
}

void	lock_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->fork[philo->index_fork1]);
	philo->data->fork_available[philo->index_fork1] = 0;
	safe_print("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->data->fork[philo->index_fork1]);
	pthread_mutex_lock(&philo->data->fork[philo->index_fork2]);
	philo->data->fork_available[philo->index_fork2] = 0;
	safe_print("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->data->fork[philo->index_fork2]);
}

void	release_forks(t_philo *philo)
{
	DEBUG && safe_print("DEBUG && Releasing forks\n", philo);
	pthread_mutex_lock(&philo->data->fork[philo->index_fork1]);
	philo->data->fork_available[philo->index_fork1] = 1;
	pthread_mutex_unlock(&philo->data->fork[philo->index_fork1]);
	pthread_mutex_lock(&philo->data->fork[philo->index_fork2]);
	philo->data->fork_available[philo->index_fork2] = 1;
	pthread_mutex_unlock(&philo->data->fork[philo->index_fork2]);
	DEBUG && safe_print("DEBUG && Releasing forks\n", philo);
}

void	print_meal_count(t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philo->data->meal);
	pthread_mutex_lock(&philo->data->print);
	printf("Meal count :");
	while (i < philo->data->philo_count)
	{
		printf("[%d]", philo->data->meal_count[i]);
		i++;
	}
	printf("\n");
	pthread_mutex_unlock(&philo->data->print);
	pthread_mutex_unlock(&philo->data->meal);
}
