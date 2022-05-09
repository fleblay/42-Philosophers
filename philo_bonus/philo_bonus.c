/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 09:49:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/09 18:13:33 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void	ft_count_sem_post(sem_t *sem, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		sem_post(sem);
		i++;
	}
}

void	ft_count_sem_wait(sem_t *sem, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		sem_wait(sem);
		i++;
	}
}

void	ft_kill_dead_monitor(t_data *data)
{
	sem_post(data->s_dead_signal);
	ft_count_sem_post(data->s_ack_msg, data->philo_count);
}

void	ft_kill_meal_monitor(t_data *data)
{
	ft_count_sem_post(data->s_meal, data->philo_count);
	ft_count_sem_post(data->s_ack_msg, data->philo_count);
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

void	*ft_meal_monitor(void *param)
{
	t_data	*data;
	int		satiated_philo;

	data = (t_data *)param;
	satiated_philo = 0;
	while (satiated_philo < data->philo_count)
	{
		sem_wait(data->s_meal);
		satiated_philo++;
	}
	//printf("\x1b[33mmeal goal achieved\x1b[0m\n");
	ft_count_sem_post(data->s_end_simu, data->philo_count);
	ft_count_sem_wait(data->s_ack_msg, data->philo_count);
	ft_kill_dead_monitor(data);
	sem_post(data->s_end_of_termination);
	return (NULL);
}

void	*ft_dead_monitor(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	sem_wait(data->s_dead_signal);
	//printf("\x1b[33ma philo is dead\x1b[0m\n");
	ft_count_sem_post(data->s_end_simu, data->philo_count);
	ft_count_sem_wait(data->s_ack_msg, data->philo_count);
	ft_kill_meal_monitor(data);
	sem_post(data->s_end_of_termination);
	return (NULL);
}

//keeping res for testing purposes
void	ft_launch_monitors(t_data *data)
{
	int	res;

	res = pthread_create(&data->meal_goal_monitor, NULL, ft_meal_monitor, data);
	if (res)
	{
		ft_sem_destroy(data, ALL);
		ft_deallocate(data);
		ft_putstr_fd("Error : thread create failure\n", 2);
		exit (1);
	}
	res = pthread_create(&data->dead_monitor, NULL, ft_dead_monitor, data);
	if (res)
	{
		ft_kill_meal_monitor(data);
		pthread_join(data->meal_goal_monitor, NULL);
		ft_sem_destroy(data, ALL);
		ft_deallocate(data);
		ft_putstr_fd("Error : thread create failure\n", 2);
		exit (1);
	}
}

void	ft_catch_fork_failure(t_data *data)
{
	ft_kill_dead_monitor(data);
	//printf("waiting for end of ack\n");
	sem_wait(data->s_end_of_termination);
	sem_post(data->s_end_of_termination);
	//printf("termination over\n");
	ft_count_sem_post(data->s_start, data->philo_count);
	//printf("waiting for childs\n");
	while (waitpid(-1, NULL, 0) > 0)
		;
	//printf("waiting for childs OVER\n");
	pthread_join(data->meal_goal_monitor, NULL);
	pthread_join(data->dead_monitor, NULL);
	ft_sem_destroy(data, ALL);
	ft_deallocate(data);
	ft_putstr_fd("Error : fork failure\n", 2);
	exit (1);
}

int	main(int ac, char *av[])
{
	t_data	data;
	int		i;

	if (ft_init_data(&data, ac, av) == 0)
		return (1);
	ft_launch_monitors(&data);
	i = 0;
	while (i < data.philo_count)
	{
		/*
		if (i == 2)
			data.philo_pid[i] = -1;
		else
		*/
			data.philo_pid[i] = fork();
		if (data.philo_pid[i] == -1)
			ft_catch_fork_failure(&data);
		if (data.philo_pid[i] == 0)
			return (ft_philo_routine(&data, i));
		else
			i++;
	}
	ft_count_sem_wait(data.s_philo_deamon, data.philo_count);
	ft_count_sem_post(data.s_start, data.philo_count);
	while (waitpid(-1, NULL, 0) > 0)
		;
	pthread_join(data.meal_goal_monitor, NULL);
	pthread_join(data.dead_monitor, NULL);
	ft_sem_destroy(&data, ALL);
	ft_deallocate(&data);
	sem_unlink("/s_last_print");
	return (0);
}
