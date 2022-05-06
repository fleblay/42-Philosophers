/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 12:59:02 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/06 11:21:46 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

int	ft_get_param(t_data *data, int ac, char *av[])
{
	int	error;

	error = 0;
	data->philo_count = ft_satoi(av[1], &error);
	if (error)
		return (ft_putstr_fd("Error : wrong philo count\n", 2), 0);
	data->ttd = ft_satoi(av[2], &error);
	if (error)
		return (ft_putstr_fd("Error : wrong time to die\n", 2), 0);
	data->tte = ft_satoi(av[3], &error);
	if (error)
		return (ft_putstr_fd("Error : wrong time to eat\n", 2), 0);
	data->tts = ft_satoi(av[4], &error);
	if (error)
		return (ft_putstr_fd("Error : wrong time to sleep\n", 2), 0);
	if (ac == 6)
	{
		data->meal_goal = ft_satoi(av[5], &error);
		if (error)
			return (ft_putstr_fd("Error : wrong meal goal\n", 2), 0);
	}
	else
		data->meal_goal = -1;
	return (1);
}

char *ft_create_self_dead_sem_name(int i)
{
	char	*name;

	name = (char *)malloc(13 * sizeof(char));
	if (!name)
		return (NULL);
	ft_strlcpy(name, "/s_philo_000", 13);
	name[9] = i / 100 + 48; 
	name[10] = i / 10 - (i / 100) * 10 + 48; 
	name[11] = i - (i / 10) * 10 - (i / 100) * 100 + 48;
	//printf("name : [%s]\n", name);
	return (name);
}

int	ft_allocate(t_data *data)
{
	int	i;

	data->philo_pid = (pid_t *)malloc(data->philo_count * sizeof(pid_t));
	if (data->philo_pid == NULL)
		return (0);
	data->s_self_dead = (sem_t **)malloc(data->philo_count * sizeof(sem_t));
	if (data->s_self_dead == NULL)
		return (free(data->philo_pid), 0);
	data->self_dead_name = (char **)malloc(data->philo_count * sizeof(char *));
	if (data->self_dead_name == NULL)
		return (free(data->philo_pid), free(data->s_self_dead), 0);
	i = 0;
	while (i < data->philo_count)
	{
		data->self_dead_name[i] = ft_create_self_dead_sem_name(i);
		if (!data->self_dead_name[i])
		{
			while (--i >= 0)
				free(data->self_dead_name[i]);
			return (free(data->philo_pid), free(data->s_self_dead),
					free(data->self_dead_name), 0);
		}
		i++;
	}
	return (1);
}

int ft_create_sem(t_data *data)
{
	data->s_print = sem_open("/s_print", O_CREAT, 0644, 0);
	if (data->s_print == SEM_FAILED)
		return (0);
	data->s_start = sem_open("/s_start", O_CREAT, 0644, 0);
	if (data->s_start == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT), 0);
	data->s_end_simu = sem_open("/s_end_simu", O_CREAT, 0644, 0);
	if (data->s_end_simu == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT | START), 0);
	data->s_meal = sem_open("/s_meal", O_CREAT, 0644, 0);
	if (data->s_meal == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT | START | END_SIM), 0);
	data->s_fork = sem_open("/s_fork", O_CREAT, 0644, data->philo_count);
	if (data->s_fork == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT | START | END_SIM | MEAL), 0);
	data->s_dead_signal = sem_open("/s_dead_signal", O_CREAT, 0644, 0);
	if (data->s_dead_signal == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT | START | END_SIM | MEAL | FORK), 0);
	data->s_ack_msg = sem_open("/s_ack_msg", O_CREAT, 0644, 0);
	if (data->s_ack_msg == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT | START | END_SIM | MEAL | FORK
				| DEAD_SIGN), 0);
	return (1);
}

int	ft_create_sem_tab(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->s_self_dead[i] = sem_open(data->self_dead_name[i], O_CREAT, 0644, 1);
		if (data->s_self_dead[i] == SEM_FAILED)
		{
			while (--i >= 0)
			{
				sem_close(data->s_self_dead[i]);
				sem_unlink(data->self_dead_name[i]);
			}
			return (0);
		}
		i++;
	}
	return (1);
}

void	ft_set_data(t_data *data)	
{
	data->dead = 0;
	data->last_start_eat = 0;
	data->last_start_sleep = 0;
	data->last_start_think = 0;
	if (data->meal_goal == 0)
		data->meal_goal_achieved = 1;
	else
		data->meal_goal_achieved = 0;
}
