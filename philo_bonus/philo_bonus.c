/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 09:49:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/06 12:35:44 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

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

void	*ft_meal_monitor(void *param)
{
	t_data		*data;
	int			satiated_philo;
	int			i;

	data = (t_data *)param;
	satiated_philo = 0;
	while (satiated_philo < data->philo_count)
	{
		sem_wait(data->s_meal);
		satiated_philo++;
	}
	printf("\x1b[33mmeal goal achieved\x1b[0m\n");
	i = 0;
	while (i < data->philo_count)
	{
		sem_post(data->s_end_simu);
		i++;
	}
	i = 0;
	while (i < data->philo_count)
	{
		sem_wait(data->s_ack_msg);
		i++;
	}
	//to kill dead_monitor - simulate dead signal and ack
	sem_post(data->s_dead_signal);
	i = 0;
	while (i < data->philo_count)
	{
		sem_post(data->s_ack_msg);
		i++;
	}
	return (NULL);
}

void	*ft_dead_monitor(void *param)
{
	t_data		*data;
	int			i;

	data = (t_data *)param;
	i = 0;
	sem_wait(data->s_dead_signal);
	printf("\x1b[33ma philo is dead\x1b[0m\n");
	while (i < data->philo_count)
	{
		sem_post(data->s_end_simu);
		i++;
	}
	i = 0;
	while (i < data->philo_count)
	{
		sem_wait(data->s_ack_msg);
		i++;
	}
	//to kill meal deamon - simulate satiated philo and ack
	i = 0;
	while (i < data->philo_count)
	{
		sem_post(data->s_meal);
		sem_post(data->s_ack_msg);
		i++;
	}
	return (NULL);
}

int	main(int ac, char *av[])
{
	t_data		data;
	pthread_t	meal_goal_monitor;
	pthread_t	dead_monitor;
	int	i;

	if (ft_init_data(&data, ac, av) == 0)
		return (1);
	i = 0;
	while (i < data.philo_count)
	{
		data.philo_pid[i] = fork();
		if (data.philo_pid[i] == -1)
		{
			ft_sem_destroy(&data, ALL);
			ft_deallocate(&data);
			return (ft_putstr_fd("Error : fork failure\n", 2), 1);
		}
		if (data.philo_pid[i] == 0)
		{
			return (ft_philo_routine(&data, i));
		}
		else
		{
			i++;
		}
	}
	printf("from parent\n");
	pthread_create(&meal_goal_monitor, NULL, ft_meal_monitor, &data);
	pthread_create(&dead_monitor, NULL, ft_dead_monitor, &data);
	// On wait la creation des deamons
	i = 0;
	while (i < data.philo_count)
	{
		sem_wait(data.s_philo_deamon);
		i++;
	}
	i = 0;
	while (i < data.philo_count)
	{
		sem_post(data.s_start);
		i++;
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	pthread_join(meal_goal_monitor, NULL);
	pthread_join(dead_monitor, NULL);
	ft_sem_destroy(&data, ALL);
	ft_deallocate(&data);
	return (0);
}
