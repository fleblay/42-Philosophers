/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 12:59:02 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/05 12:46:08 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

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
	data->philo_pid = (pid_t *)malloc(data->philo_count * sizeof(pid_t));
	if (data->philo_pid == NULL)
		return (0);
	return (1);
}

int ft_open_sem(t_data *data)
{
	data->s_print = sem_open("/s_print", O_CREAT, 0644, 1);
	if (data->s_print == SEM_FAILED)
		return (0);
	data->s_start = sem_open("/s_start", O_CREAT, 0644, 0);
	if (data->s_start == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT), 0);
	data->s_dead = sem_open("/s_dead", O_CREAT, 0644, 0);
	if (data->s_dead == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT | START), 0);
	data->s_meal = sem_open("/s_meal", O_CREAT, 0644, 0);
	if (data->s_meal == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT | START | DEAD), 0);
	data->s_fork = sem_open("/s_meal", O_CREAT, 0644, data->philo_count);
	if (data->s_fork == SEM_FAILED)
		return (ft_sem_destroy(data, PRINT | START | DEAD | MEAL), 0);
	return (1);
}

void	ft_sem_destroy(t_data *data, int flags)
{
	if (flags & 1 << 0)
	{
		sem_close(data->s_print);
		sem_unlink("/s_print");
	}
	if (flags & 1 << 1)
	{
		sem_close(data->s_start);
		sem_unlink("/s_start");
	}
	if (flags & 1 << 2)
	{
		sem_close(data->s_dead);
		sem_unlink("/s_dead");
	}
	if (flags & 1 << 3)
	{
		sem_close(data->s_meal);
		sem_unlink("/s_meal");
	}
	if (flags & 1 << 4)
	{
		sem_close(data->s_fork);
		sem_unlink("/s_fork");
	}
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
