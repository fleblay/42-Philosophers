/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:01:51 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/02 16:04:22 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stddef.h>

void	*philo_routine(void *philo_struct)
{
	t_philo	*philo;
	
	philo = (t_philo *)philo_struct;
	pthread_mutex_lock(&philo->data->m_start);
	philo->start_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->m_start);
	if (philo->start_time == 0)
		return (NULL);
	return (NULL);
}
