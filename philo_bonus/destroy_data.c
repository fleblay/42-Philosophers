/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 13:02:13 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 16:10:12 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void	ft_deallocate(t_data *data)
{
	free(data->philo);
	free(data->fork_available);
	free(data->thread);
	free(data->m_fork);
	free(data->meal_count);
}

void	ft_mutex_destroy(t_data *data, int flags)
{
	int	i;

	i = 0;
	if (flags & 1 << 0)
	{
		while (i < data->philo_count)
			pthread_mutex_destroy(&data->m_fork[i++]);
	}
	if (flags & 1 << 1)
		pthread_mutex_destroy(&data->m_print);
	if (flags & 1 << 2)
		pthread_mutex_destroy(&data->m_start);
	if (flags & 1 << 3)
		pthread_mutex_destroy(&data->m_time);
	if (flags & 1 << 4)
		pthread_mutex_destroy(&data->m_dead);
	if (flags & 1 << 5)
		pthread_mutex_destroy(&data->m_check_fork);
	if (flags & 1 << 6)
		pthread_mutex_destroy(&data->m_meal);
}
