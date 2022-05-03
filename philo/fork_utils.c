/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 14:43:52 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 15:40:12 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	ft_grab_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_fork[philo->left_fork]);
	philo->data->fork_available[philo->left_fork] = 0;
	s_print(philo, "has taken a fork", 0);
	pthread_mutex_unlock(&philo->data->m_fork[philo->left_fork]);
	pthread_mutex_lock(&philo->data->m_fork[philo->right_fork]);
	philo->data->fork_available[philo->right_fork] = 0;
	s_print(philo, "has taken a fork", 0);
	pthread_mutex_unlock(&philo->data->m_fork[philo->right_fork]);
	return (1);
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
