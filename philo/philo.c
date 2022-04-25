/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:17:42 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/25 18:31:58 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

int		get_sim_duration(void)
{
	static time_t		start_sec = 0;
	static suseconds_t	start_usec = 0;
	struct timeval		time;
	int					time_stamp;

	if (start_sec == 0 && start_usec == 0)
	{
		if (gettimeofday(&time, NULL) == -1)
			return (-1);
		start_sec = time.tv_sec;
		start_usec = time.tv_usec;
		printf("Setting start of sim [%ld:%ld]\n", start_sec, start_usec);
		return (0);
	}
	else if (gettimeofday(&time, NULL) == -1)
		return (-1);
	time_stamp = (time.tv_sec - start_sec) * 1000
		+ (time.tv_usec - start_usec) / 1000;
	return (time_stamp);
}

void	safe_print(int id, char *txt, pthread_mutex_t *print, int monitor)
{
	pthread_mutex_lock(print);
	printf("[%d] ", get_sim_duration());
	if (monitor)
		printf("[MONITOR]");
	if (id >= 0)
		printf("Philo %d ", id);
	printf("%s", txt);
	pthread_mutex_unlock(print);
}

void	*philo_routine(void *phil)
{
	t_philo	*philo;
	philo = (t_philo *)phil;
	safe_print(philo->id, "is alive\n", &philo->data->print, 0);
	while (philo->dead == -1)
	{
		//check if one died
		pthread_mutex_lock(&philo->data->server_dead_philo);
		if (philo->data->dead_philo != -1)
		{
			philo->dead = philo->data->dead_philo;
			pthread_mutex_unlock(&philo->data->server_dead_philo);
			safe_print(philo->id, "Found out dead philo before trying to init com\n", &philo->data->print, 0);
			break ;
		}
		pthread_mutex_unlock(&philo->data->server_dead_philo);
		//check if one died
		//fake dead
		if (philo->id == 1 && get_sim_duration() > 3000)
		{
			safe_print(philo->id, "Faking Death\n", &philo->data->print, 0);
			pthread_mutex_lock(&philo->data->server_dead_philo);
			philo->data->dead_philo = philo->id;
			philo->dead = philo->id;
			pthread_mutex_unlock(&philo->data->server_dead_philo);
		}
		//fake dead
		safe_print(philo->id, "Trying to get com\n", &philo->data->print, 0);
		//TODO : Boucle pour savoir si la com est dispo. On quitte si on meurt pendant cette phase de try ou si un philo est mort
		pthread_mutex_lock(&philo->data->server_com);
		safe_print(philo->id, "Com ok\n", &philo->data->print, 0);
		pthread_mutex_lock(&philo->data->server_request);
		philo->data->request = philo->id;
		safe_print(philo->id, "Request made\n", &philo->data->print, 0);
		pthread_mutex_unlock(&philo->data->server_request);
		philo->answer = -1;
		while (philo->dead == -1 && philo->answer == -1)
		{
			//check if one died
			pthread_mutex_lock(&philo->data->server_dead_philo);
			if (philo->data->dead_philo != -1)
				philo->dead = philo->data->dead_philo;
			pthread_mutex_unlock(&philo->data->server_dead_philo);
			//check if one died
			pthread_mutex_lock(&philo->data->server_answer);
			if (philo->data->answer == -1)
			{
				safe_print(philo->id, "Waiting for answer\n", &philo->data->print, 0);
				pthread_mutex_unlock(&philo->data->server_answer);
				//check if one died
				usleep(80);
				continue ;
			}
			safe_print(philo->id, "Answer is given after waiting\n", &philo->data->print, 0);
			philo->answer = philo->data->answer;
			philo->data->answer = -1;
			pthread_mutex_unlock(&philo->data->server_answer);
		}
		safe_print(philo->id, "Answer is given\n", &philo->data->print, 0);
		pthread_mutex_unlock(&philo->data->server_com);
		safe_print(philo->id, "Releasing com\n", &philo->data->print, 0);
		if (philo->answer == 0)
		{
			safe_print(philo->id, "answer is KO\n", &philo->data->print, 0);
			usleep(10);
			safe_print(philo->id, "Finished waiting after KO\n", &philo->data->print, 0);
		}
		else if (philo->answer > 0)
		{
			safe_print(philo->id, "answer is OK\n", &philo->data->print, 0);
			usleep(2000);
			safe_print(philo->id, "Finished waiting after OK\n", &philo->data->print, 0);
		}
		/*
		else if (philo->answer == -2)
		{
			safe_print(philo->id, "answer is DEAD philo\n", &philo->data->print, 0);
			break ;
		}
		*/
	}
	if (philo->dead == philo->id)
		safe_print(philo->id, "quiting because of i am dead\n", &philo->data->print, 0);
	else if (philo->dead != -1)
		safe_print(philo->id, "quiting because of dead peer\n", &philo->data->print, 0);
	return (phil);
}

int	main(int ac, char *av[])
{
	long		i;
	static int	ok = 0;
	t_data	data;
	t_philo	*philo;

	(void)ac;
	pthread_mutex_init(&data.fork1, NULL);
	pthread_mutex_init(&data.fork2, NULL);
	pthread_mutex_init(&data.fork3, NULL);
	pthread_mutex_init(&data.server_request, NULL);
	pthread_mutex_init(&data.server_answer, NULL);
	pthread_mutex_init(&data.server_com, NULL);
	pthread_mutex_init(&data.server_dead_philo, NULL);
	pthread_mutex_init(&data.print, NULL);
	data.request_pending = -1;
	data.request = -1;
	data.answer = -1;
	data.dead_philo = -1;
	data.run = 1;

	i = 0;
	get_sim_duration();
	data.thread = (pthread_t *)malloc(atoi(av[1]) * sizeof(pthread_t));
	philo = (t_philo *)malloc(atoi(av[1]) * sizeof(t_philo));
	while (i < atoi(av[1]))
	{
		philo[i].id = i;
		philo[i].answer = -1;
		philo[i].dead = -1;
		philo[i].data = &data;
		pthread_create(&data.thread[i], NULL, philo_routine, &philo[i]);
		i++;
	}
	while (data.run)
	{
		data.request_pending = -1;
		while (data.run && data.request_pending == -1)
		{
			pthread_mutex_lock(&data.server_request);
			if (data.request == -1)
			{
				safe_print(-1, "Monitor waiting for request\n", &data.print, 1);
				pthread_mutex_unlock(&data.server_request);
				usleep(20);
				continue ;
			}
			data.request_pending = data.request;
			data.request = -1;
			safe_print(data.request_pending, "Monitor has a request\n", &data.print, 1);
			pthread_mutex_unlock(&data.server_request);
		}
		pthread_mutex_lock(&data.server_answer);
		//Check if dead philo
		pthread_mutex_lock(&data.server_dead_philo);
		if (data.dead_philo != -1)
			data.run = 0;
		pthread_mutex_unlock(&data.server_dead_philo);
		//Check if dead philo
		ok +=1;
		data.answer = data.run * ok + (!data.run) * (-1);
		if (data.answer > 0)
			safe_print(data.request_pending, "Monitor gives OK to request\n", &data.print, 1);
		else if (data.answer == 0)
			safe_print(data.request_pending, "Monitor gives KO to request\n", &data.print, 1);
		else
			safe_print(data.request_pending, "Monitor gives DEAD signal to request\n", &data.print, 1);
		pthread_mutex_unlock(&data.server_answer);
	}
	safe_print(data.dead_philo, "Monitor has received RIP status\n", &data.print, 1);
	i = 0;
	while (i < atoi(av[1]))
	{
		pthread_join(data.thread[i], NULL);
		i++;
	}
	safe_print(0 , "Finished waiting threads\n", &data.print, 1);
	pthread_mutex_destroy(&data.fork1);
	pthread_mutex_destroy(&data.fork2);
	pthread_mutex_destroy(&data.fork3);
	pthread_mutex_destroy(&data.server_request);
	pthread_mutex_destroy(&data.server_answer);
	pthread_mutex_destroy(&data.server_com);
	pthread_mutex_destroy(&data.server_dead_philo);
	pthread_mutex_destroy(&data.print);
	printf("Fin Simu\n");
	return (1);
}
