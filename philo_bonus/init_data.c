/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 12:59:02 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/11 15:49:38 by fred             ###   ########.fr       */
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

void	ft_set_data(t_data *data)
{
	data->start_time = 0;
	data->dead = 0;
	data->go_on = 1;
	data->last_start_eat = 0;
	data->last_start_sleep = 0;
	data->last_start_think = 0;
}

int	ft_init_data(t_data *data, int ac, char *av[])
{
	if (ac != 5 && ac != 6)
		return (ft_putstr_fd("Error : wrong arg count\n", 2), 0);
	if (!ft_get_param(data, ac, av))
		return (ft_putstr_fd("Error : wrong parameter\n", 2), 0);
	if (data->philo_count > 200 || data->ttd < 60
		|| data->tte < 60 || data->tts < 60)
		return (ft_putstr_fd("Error : out of bound parameter\n", 2), 0);
	if (!ft_allocate(data))
		return (ft_putstr_fd("Error : allocate failure\n", 2), 0);
	ft_sem_unlink(data, ALL);
	if (!ft_create_sem_tab(data) || !ft_create_sem(data))
		return (ft_deallocate(data),
			ft_putstr_fd("Error : init semaphore failure\n", 2), 0);
	ft_set_data(data);
	return (1);
}
