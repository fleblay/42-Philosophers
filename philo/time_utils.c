/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:25:00 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 11:24:07 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stddef.h>
#include <unistd.h>
#include "philo.h"

unsigned long	ft_get_time(void)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

int	ft_self_is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_time);
	philo->current_time = ft_get_time() - philo->start_time;;
	pthread_mutex_unlock(&philo->data->m_time);
	if (philo->current_time - philo->last_start_eat > philo->ttd)
	{
		philo->dead = 1;
		pthread_mutex_lock(&philo->data->m_check);
		philo->data->dead_philo = 1 ;
		pthread_mutex_unlock(&philo->data->m_check);
		s_print(philo, "died", 1);
		return (1);
	}
	return (0);
}

int	ft_someone_is_dead(t_philo *philo)
{
	int	dead;

	dead = 0;
	dead += ft_self_is_dead(philo);
	pthread_mutex_lock(&philo->data->m_check);
	dead += philo->data->dead_philo;
	pthread_mutex_unlock(&philo->data->m_check);
	if (dead)
		return (1);
	return (0);
}

int	ft_eat_for_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_time);
	philo->current_time = ft_get_time() - philo->start_time;;
	pthread_mutex_unlock(&philo->data->m_time);
	s_print(philo, "is eating", 0);
	philo->last_start_eat = philo->current_time;
	while (philo->current_time - philo->last_start_eat < philo->tte)
	{
		if (ft_someone_is_dead(philo))
		{
			ft_release_forks(philo);
			return (0);
		}
		else
			usleep(500);
		pthread_mutex_lock(&philo->data->m_time);
		philo->current_time = ft_get_time() - philo->start_time;;
		pthread_mutex_unlock(&philo->data->m_time);
	}
	return (1);
}

int	ft_sleep_for_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_time);
	philo->current_time = ft_get_time() - philo->start_time;;
	pthread_mutex_unlock(&philo->data->m_time);
	s_print(philo, "is sleeping", 0);
	philo->last_start_sleep = philo->current_time;
	while (philo->current_time - philo->last_start_sleep < philo->tts)
	{
		if (ft_someone_is_dead(philo))
			return (0);
		else
			usleep(500);
		pthread_mutex_lock(&philo->data->m_time);
		philo->current_time = ft_get_time() - philo->start_time;;
		pthread_mutex_unlock(&philo->data->m_time);
	}
	return (1);
}

