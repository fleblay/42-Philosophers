/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:01:51 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/02 18:41:31 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stddef.h>
#include <unistd.h>

int	ft_get_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_fork[philo->left_fork]);
	if (philo->data->fork_available[philo->left_fork] == 1)
	{
		pthread_mutex_lock(&philo->data->m_fork[philo->right_fork]);
		if (philo->data->fork_available[philo->right_fork] == 1)
		{
			philo->data->fork_available[philo->left_fork] = 0;
			s_print(philo, "has taken a fork");
			philo->data->fork_available[philo->right_fork] = 0;
			s_print(philo, "has taken a fork");
			pthread_mutex_unlock(&philo->data->m_fork[philo->right_fork]);
			pthread_mutex_unlock(&philo->data->m_fork[philo->left_fork]);
			return (1);
		}
		pthread_mutex_unlock(&philo->data->m_fork[philo->right_fork]);
	}
	pthread_mutex_unlock(&philo->data->m_fork[philo->left_fork]);
	return (0);
}

int	ft_release_forks(t_philo *philo)
{

	pthread_mutex_lock(&philo->data->m_fork[philo->left_fork]);
	philo->data->fork_available[philo->left_fork] = 1;
	pthread_mutex_unlock(&philo->data->m_fork[philo->left_fork]);
	pthread_mutex_lock(&philo->data->m_fork[philo->right_fork]);
	philo->data->fork_available[philo->right_fork] = 1;
	pthread_mutex_unlock(&philo->data->m_fork[philo->right_fork]);
	return (0);
}

void	*philo_routine(void *philo_struct)
{
	t_philo	*philo;
	
	philo = (t_philo *)philo_struct;
	pthread_mutex_lock(&philo->data->m_start);
	philo->start_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->m_start);
	if (philo->start_time == 0)
		return (NULL);
	if (philo->id % 2 == 0)
		usleep((philo->tte - 10)* 1000);
	while (philo->dead == 0)
	{
		if (ft_get_forks(philo))
		{
			s_print(philo, "is eating");
			usleep(philo->tte * 1000);
			ft_release_forks(philo);
			s_print(philo, "is sleeping");
			usleep(philo->tts * 1000);
			s_print(philo, "is thinking");
			if (philo->philo_count % 3 == 0)
				usleep((philo->ttd - philo->tte - philo->tts - 10)* 1000);
		}
		else
			usleep(50);
	}
	return (NULL);
}
