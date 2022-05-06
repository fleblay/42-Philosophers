/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 14:49:15 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/06 11:22:28 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>

void	ft_sem_close(t_data *data, int flags)
{
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
}

void	ft_sem_unlink(int flags)
{
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
}

void	ft_sem_destroy(t_data *data, int flags)
{
	ft_sem_close(data, flags);
	ft_sem_unlink(flags);
	//unlink to add pour tab
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
