/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 09:49:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 13:06:16 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

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
