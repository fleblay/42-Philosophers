/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:17:42 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/28 12:47:20 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

void	*philo_routine(void *philosopher)
{
	t_philo	*philo;
	philo = (t_philo *)philosopher;
	pthread_mutex_lock(&philo->data->start);
	pthread_mutex_unlock(&philo->data->start);
	// Remplacer par une fonction clean du monitor qui donne KO
	if (philo->id % 2 == 0)
		usleep(8000);
	// Remplacer par une fonction clean du monitor qui donne KO
	while (philo->dead == -1 && philo->meal_goal_achieved == 0)
	{
		wait_for_com_token(philo);
		if (philo->dead != -1)
			break ;
		grab_com_and_place_request(philo);
		if (philo->dead != -1)
		{
			release_com_token_and_com(philo);
			break ;
		}
		wait_for_answer(philo);
		if (philo->dead != -1)
		{
			release_com_token_and_com(philo);
			break ;
		}
		if (philo->answer == 0)
		{
			release_com_token_and_com(philo);
			//safe_print(philo->id, "answer is KO\n", &philo->data->print, 0);
			usleep(100);
			//safe_print(philo->id, "Finished waiting after KO\n", &philo->data->print, 0);
		}
		else if (philo->answer > 0)
		{
			//safe_print(philo->id, "answer is OK\n", &philo->data->print, 0);
			lock_forks(philo);
			release_com_token_and_com(philo);
			if (philo->dead == -1)
				eat_for_time(philo);
			release_forks(philo);
			if (philo->dead == -1)
				sleep_for_time(philo);
		}
		else if (philo->answer == -1)
		{
			release_com_token_and_com(philo);
			//safe_print(philo->id, "answer is someone died\n", &philo->data->print, 0);
		}
		else if (philo->answer == -2)
		{
			release_com_token_and_com(philo);
			philo->meal_goal_achieved = 1;
			//safe_print(philo->id, "answer is meal count achieved\n", &philo->data->print, 0);
		}
	}
	if (philo->meal_goal_achieved == 1)
	{
		//safe_print(philo->id, "quiting because meal count achieved\n", &philo->data->print, 0);
	}
	else if (philo->dead == philo->id)
	{
		safe_print(philo->id, "died\n", &philo->data->print, 0);
	}
	else if (philo->dead != -1)
	{
		//safe_print(philo->id, "quiting because of dead peer\n", &philo->data->print, 0);
	}
	return (NULL);
}

int	main(int ac, char *av[])
{
	t_data	data;
	
	if (ac != 5 && ac != 6)
		return (printf("Wrong arg count\n"), 1);
	if (!init_data(&data, ac, av))
		return (printf("Malloc error while init\n"), 1);
	init_philo(&data);
	//get_sim_duration();
	launch_philo(&data);
	while (data.run)
	{
		get_request(&data);
		pthread_mutex_lock(&data.server_answer);
		if (monitor_someone_is_dead(&data) || meal_goal_achieved(&data))
			data.run = 0;
		data.answer = data.run * check_available_forks(&data) + data.philo_is_dead * (-1) + data.meal_goal_achieved * (-2);
		if (data.answer > 0)
		{
			//safe_print(data.request_pending, "Monitor gives OK to request\n", &data.print, 1);
		}
		else if (data.answer == 0)
		{
			//safe_print(data.request_pending, "Monitor gives KO to request\n", &data.print, 1);
		}
		else if (data.answer == -1)
		{
			//safe_print(data.request_pending, "Monitor gives DEAD signal to request\n", &data.print, 1);
		}
		else if (data.answer == -2)
		{
			//safe_print(data.request_pending, "Monitor gives meal count achieved\n", &data.print, 1);
		}
		pthread_mutex_unlock(&data.server_answer);
	}
	if (!data.run)
	{
		//safe_print(data.dead_philo, "Monitor has received RIP status\n", &data.print, 1);
	}
	else
	{
		//safe_print(data.dead_philo, "Meal count achieved\n", &data.print, 1);
	}
	cleanup(&data);
	return (1);
}
