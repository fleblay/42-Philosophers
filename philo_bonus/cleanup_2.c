/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fred <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 12:46:12 by fred              #+#    #+#             */
/*   Updated: 2022/05/11 15:50:41 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>

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

void	ft_cleanup_exit_error(t_data *data, char *msg)
{
	ft_sem_destroy(data, ALL);
	ft_deallocate(data);
	ft_putstr_fd(msg, 2);
	exit (1);
}
