/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 09:49:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/11 14:50:02 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

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
	printf("\x1b[33mmeal goal achieved\x1b[0m\n");
	ft_count_sem_post(data->s_end_simu, data->philo_count);
	ft_count_sem_wait(data->s_ack_msg, data->philo_count);
	ft_kill_dead_monitor(data);
	ft_count_sem_post(data->s_end_of_termination, data->philo_count);
	printf("\x1b[33mmeal_monitor returning\x1b[0m\n");
	return (NULL);
}

void	*ft_dead_monitor(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	sem_wait(data->s_dead_signal);
	printf("\x1b[33ma philo is dead\x1b[0m\n");
	ft_count_sem_post(data->s_end_simu, data->philo_count);
	ft_count_sem_wait(data->s_ack_msg, data->philo_count);
	ft_kill_meal_monitor(data);
	ft_count_sem_post(data->s_end_of_termination, data->philo_count);
	printf("\x1b[33mdead_monitor returning\x1b[0m\n");
	return (NULL);
}

//keeping res for testing purposes
void	ft_launch_monitors(t_data *data)
{
	int	res;

	res = pthread_create(&data->meal_goal_monitor, NULL, ft_meal_monitor, data);
	if (res)
		ft_cleanup_exit_error(data, "Error : fork failure\n");
	res = pthread_create(&data->dead_monitor, NULL, ft_dead_monitor, data);
	if (res)
	{
		ft_kill_meal_monitor(data);
		pthread_join(data->meal_goal_monitor, NULL);
		ft_cleanup_exit_error(data, "Error : fork failure\n");
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
	ft_cleanup_exit_error(data, "Error : fork failure\n");
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
	// switch a confirmer de l'emplacement des join avant les waits
	pthread_join(data.meal_goal_monitor, NULL);
	pthread_join(data.dead_monitor, NULL);
	ft_unlock_solo_philo(&data);
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (ft_sem_destroy(&data, ALL), ft_deallocate(&data), 0);
}
