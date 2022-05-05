/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 09:49:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/05 16:35:36 by fle-blay         ###   ########.fr       */
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
	ft_sem_unlink(ALL);
	if (!ft_create_sem(data))
		return (ft_putstr_fd("Error : init semaphore failure\n", 2), 0);
	ft_set_data(data);	
	return (1);
}

void	ft_kill_them_all(t_data *data, int i)
{
	//a faire propreement avec un sempost sur dead;
	int	index;

	while (index < i)
	{
		kill(data->philo_pid[index], SIGINT);
		index++;
	}
}

void	*ft_meal_monitor(void *param)
{
	t_data		*data;
	int			satiated_philo;

	data = (t_data *)param;
	satiated_philo = 0;
	while (satiated_philo != data->philo_count)
	{
		sem_wait(data->s_meal);
		satiated_philo++;
	}
	sem_post(data->s_dead);
	return (NULL);
}

int	main(int ac, char *av[])
{
	t_data		data;
	pthread_t	meal_goal_monitor;
	int	i;

	if (ft_init_data(&data, ac, av) == 0)
		return (1);
	i = 0;
	while (i < data.philo_count)
	{
		data.philo_pid[i] = fork();
		if (data.philo_pid[i] == -1)
		{
			ft_kill_them_all(&data, i);
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
	pthread_detach(meal_goal_monitor);
	sem_post(data.s_start);
	while (waitpid(-1, NULL, 0) > 0)
		;
	ft_sem_destroy(&data, ALL);
	ft_deallocate(&data);
	return (0);
}
