/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 09:49:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/11 11:51:39 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

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
		return (ft_putstr_fd("Error : init failure\n", 2), 0);
	if (!ft_mutex_init(data))
		return (ft_deallocate(data),
			ft_putstr_fd("Error : init mutexes\n", 2), 0);
	ft_set_data(data);
	return (1);
}

int	main(int ac, char *av[])
{
	t_data	data;
	int		error;

	if (ft_init_data(&data, ac, av) == 0)
		return (1);
	error = (ft_launch_philo(&data) == 0);
	pthread_mutex_unlock(&data.m_start);
	ft_join_philo(&data);
	ft_mutex_destroy(&data, ALL);
	ft_deallocate(&data);
	return (error);
}
