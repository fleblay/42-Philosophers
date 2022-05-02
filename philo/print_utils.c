/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 17:38:04 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/02 18:44:30 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include "stdio.h"

void	s_print(t_philo *philo, char *msg)
{
	unsigned long	now;

	pthread_mutex_lock(&philo->data->m_time);
	now = ft_get_time();
	pthread_mutex_unlock(&philo->data->m_time);
	pthread_mutex_lock(&philo->data->m_print);
	printf("%ld %d %s\n", now - philo->start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->m_print);
}
