/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 17:38:04 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/11 15:30:51 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include "stdio.h"

void	s_print(t_philo *philo, char *msg, int dead)
{
	static int		last_msg = 0;
	unsigned long	now;

	now = ft_get_time();
	pthread_mutex_lock(&philo->data->m_print);
	if (!last_msg)
		printf("%ld %d %s\n", now - philo->start_time, philo->id, msg);
	if (dead)
		last_msg = 1;
	pthread_mutex_unlock(&philo->data->m_print);
}
