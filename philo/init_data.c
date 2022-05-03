/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 12:59:02 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 12:56:19 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>

static int	ft_get_param(t_data *data, int ac, char *av[])
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

static int	ft_allocate(t_data *data)
{
	data->philo = malloc(data->philo_count * sizeof(t_philo));
	if (!data->philo)
		return (ft_putstr_fd("Error : malloc\n", 2), 0);
	data->fork_available = malloc(data->philo_count * sizeof(int));
	if (!data->fork_available)
		return (free(data->philo), ft_putstr_fd("Error : malloc\n", 2), 0);
	data->thread = malloc(data->philo_count * sizeof(pthread_t));
	if (!data->thread)
		return (free(data->philo), free(data->fork_available),
			ft_putstr_fd("Error : malloc\n", 2), 0);
	data->m_fork = malloc(data->philo_count * sizeof(pthread_mutex_t));
	if (!data->m_fork)
		return (free(data->philo), free(data->fork_available),
			free(data->thread), ft_putstr_fd("Error : malloc\n", 2), 0);
	data->meal_count = malloc(data->philo_count * sizeof(int));
	if (!data->meal_count)
		return (free(data->philo), free(data->fork_available),
			free(data->thread), free(data->m_fork),
			ft_putstr_fd("Error : malloc\n", 2), 0);
	return (1);
}

static int	ft_mutex_init(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->m_fork[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->m_fork[i]);
			return (0);
		}
		i++;
	}
	if (pthread_mutex_init(&data->m_print, NULL))
		return (ft_mutex_destroy(data, FORK_TAB), 0);
	if (pthread_mutex_init(&data->m_start, NULL))
		return (ft_mutex_destroy(data, FORK_TAB | PRINT), 0);
	if (pthread_mutex_init(&data->m_time, NULL))
		return (ft_mutex_destroy(data, FORK_TAB | PRINT | START), 0);
	if (pthread_mutex_init(&data->m_dead, NULL))
		return (ft_mutex_destroy(data, FORK_TAB | PRINT | START | TIME), 0);
	if (pthread_mutex_init(&data->m_check_fork, NULL))
		return (ft_mutex_destroy(data,
			FORK_TAB | PRINT | START | TIME | DEAD), 0);
	if (pthread_mutex_init(&data->m_meal, NULL))
		return (ft_mutex_destroy(data,
			FORK_TAB | PRINT | START | TIME | DEAD | CHECK_FORK), 0);
	return (1);
}

static void	ft_set_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->fork_available[i] = 1;
		data->meal_count[i] = 0;
		i++;
	}
	data->dead_philo = 0;
}

int	ft_init_data(t_data *data, int ac, char *av[])
{
	if (ac != 5 && ac != 6)
		return (ft_putstr_fd("Error : wrong arg count\n", 2), 0);
	if (!ft_get_param(data, ac, av) || !ft_allocate(data))
		return (ft_putstr_fd("Error : wrong parameter \n", 2), 0);
	if (!ft_mutex_init(data))
		return (ft_deallocate(data),
			ft_putstr_fd("Error : init mutexes \n", 2), 0);
	ft_set_data(data);
	return (1);
}
