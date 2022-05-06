/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:49:15 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/06 12:25:21 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>

void	ft_sem_close(t_data *data, int flags)
{
	int	i;

	if (flags & 1 << 0)
		sem_close(data->s_print);
	if (flags & 1 << 1)
		sem_close(data->s_start);
	if (flags & 1 << 2)
		sem_close(data->s_end_simu);
	if (flags & 1 << 3)
		sem_close(data->s_meal);
	if (flags & 1 << 4)
		sem_close(data->s_fork);
	if (flags & 1 << 5)
		sem_close(data->s_dead_signal);
	if (flags & 1 << 6)
		sem_close(data->s_ack_msg);
	if (flags & 1 << 7)
		sem_close(data->s_philo_deamon);
	i = 0;
	if (flags & 1 << 8)
	{
		while (i < data->philo_count)
		{
			sem_close(data->s_self_dead[i]);
			i++;
		}
	}
}

void	ft_sem_unlink(t_data *data, int flags)
{
	int	i;

	if (flags & 1 << 0)
		sem_unlink("/s_print");
	if (flags & 1 << 1)
		sem_unlink("/s_start");
	if (flags & 1 << 2)
		sem_unlink("/s_end_simu");
	if (flags & 1 << 3)
		sem_unlink("/s_meal");
	if (flags & 1 << 4)
		sem_unlink("/s_fork");
	if (flags & 1 << 5)
		sem_unlink("/s_dead_signal");
	if (flags & 1 << 6)
		sem_unlink("/s_ack_msg");
	if (flags & 1 << 7)
		sem_unlink("/s_philo_deamon");
	i = 0;
	if (flags & 1 << 8)
	{
		while (i < data->philo_count)
		{
			sem_unlink(data->self_dead_name[i]);
			i++;
		}
	}
}

void	ft_sem_destroy(t_data *data, int flags)
{
	ft_sem_close(data, flags);
	ft_sem_unlink(data, flags);
}

void	ft_deallocate(t_data *data)
{
	int	i;
	free(data->philo_pid);
	free(data->s_self_dead);
	i = 0;
	while (i < data->philo_count)
	{
		free(data->self_dead_name[i]);
		i++;
	}
	free(data->self_dead_name);
}
