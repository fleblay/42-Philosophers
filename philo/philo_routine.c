/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:01:51 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 11:28:44 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stddef.h>
#include <unistd.h>

int	ft_available_forks(t_philo *philo)
{
	int	free_fork;

	free_fork = 0;
	if (philo->left_fork == philo->right_fork)
		return (0);
	pthread_mutex_lock(&philo->data->m_fork[philo->left_fork]);
	if (philo->data->fork_available[philo->left_fork] == 1)
		++free_fork;
	pthread_mutex_unlock(&philo->data->m_fork[philo->left_fork]);
	pthread_mutex_lock(&philo->data->m_fork[philo->right_fork]);
	if (philo->data->fork_available[philo->right_fork] == 1)
		++free_fork;
	pthread_mutex_unlock(&philo->data->m_fork[philo->right_fork]);
	if (free_fork == 2)
		return (1);
	return (0);
}

void	ft_grab_forks(t_philo *philo)
{

	pthread_mutex_lock(&philo->data->m_fork[philo->left_fork]);
	philo->data->fork_available[philo->left_fork] = 0;
	s_print(philo, "has taken a fork", 0);
	pthread_mutex_unlock(&philo->data->m_fork[philo->left_fork]);
	pthread_mutex_lock(&philo->data->m_fork[philo->right_fork]);
	philo->data->fork_available[philo->right_fork] = 0;
	s_print(philo, "has taken a fork", 0);
	pthread_mutex_unlock(&philo->data->m_fork[philo->right_fork]);
}

void	ft_release_forks(t_philo *philo)
{

	pthread_mutex_lock(&philo->data->m_fork[philo->left_fork]);
	philo->data->fork_available[philo->left_fork] = 1;
	pthread_mutex_unlock(&philo->data->m_fork[philo->left_fork]);
	pthread_mutex_lock(&philo->data->m_fork[philo->right_fork]);
	philo->data->fork_available[philo->right_fork] = 1;
	pthread_mutex_unlock(&philo->data->m_fork[philo->right_fork]);
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
		if (ft_someone_is_dead(philo))
			break ;
		pthread_mutex_lock(&philo->data->m_check);
		philo->can_eat = ft_available_forks(philo);
		pthread_mutex_unlock(&philo->data->m_check);
		if (philo->can_eat == 1)
		{
			pthread_mutex_lock(&philo->data->m_check);
			ft_grab_forks(philo);
			pthread_mutex_unlock(&philo->data->m_check);
			if (ft_eat_for_time(philo) == 0)
				break ;
			ft_release_forks(philo);
			if (ft_sleep_for_time(philo) == 0)
				break ;
			s_print(philo, "is thinking", 0);
			if (philo->ttd - philo->tte - philo->tts > 10)
				usleep((philo->ttd - philo->tte - philo->tts - 10) * 1000);
		}
		else
			usleep(100);
	}
	return (NULL);
}
