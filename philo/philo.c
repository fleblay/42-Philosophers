/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:17:42 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/27 10:47:54 by fle-blay         ###   ########.fr       */
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
	safe_print(philo->id, "is alive\n", &philo->data->print, 0);
	while (philo->dead == -1)
	{
		wait_for_com_token(philo);
		grab_com_and_place_request(philo);
		wait_for_answer(philo);
		if (philo->answer == 0)
		{
			release_com_token_and_com(philo);
			safe_print(philo->id, "answer is KO\n", &philo->data->print, 0);
			usleep(10);
			safe_print(philo->id, "Finished waiting after KO\n", &philo->data->print, 0);
		}
		else if (philo->answer > 0)
		{
			safe_print(philo->id, "answer is OK\n", &philo->data->print, 0);
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
			safe_print(philo->id, "answer is someone diead\n", &philo->data->print, 0);
		}
	}
	if (philo->dead == philo->id)
		safe_print(philo->id, "quiting because of i am dead\n", &philo->data->print, 0);
	else if (philo->dead != -1)
		safe_print(philo->id, "quiting because of dead peer\n", &philo->data->print, 0);
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
	get_sim_duration();
	launch_philo(&data);
	while (data.run)
	{
		get_request(&data);
		pthread_mutex_lock(&data.server_answer);
		if (monitor_someone_is_dead(&data))
			data.run = 0;
		data.answer = data.run * check_available_forks(&data) + (!data.run) * (-1);
		if (data.answer > 0)
			safe_print(data.request_pending, "Monitor gives OK to request\n", &data.print, 1);
		else if (data.answer == 0)
			safe_print(data.request_pending, "Monitor gives KO to request\n", &data.print, 1);
		else
			safe_print(data.request_pending, "Monitor gives DEAD signal to request\n", &data.print, 1);
		pthread_mutex_unlock(&data.server_answer);
	}
	safe_print(data.dead_philo, "Monitor has received RIP status\n", &data.print, 1);
	cleanup(&data);
	return (1);
}
