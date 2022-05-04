/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 15:32:43 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 15:40:56 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_set_start_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_start);
	philo->start_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->m_start);
}

void	ft_set_goal_achieved(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_meal);
	philo->meal_goal_achieved = ft_meal_goal_achieved(philo);
	pthread_mutex_unlock(&philo->data->m_meal);
}

void	ft_set_dead_status(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_dead);
	philo->dead += philo->data->dead_philo;
	pthread_mutex_unlock(&philo->data->m_dead);
	philo->dead += ft_self_is_dead(philo);
}

void	ft_check_if_can_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->m_check_fork);
	philo->can_eat = ft_available_forks(philo);
	pthread_mutex_unlock(&philo->data->m_check_fork);
}

int	ft_preliminary_checks(t_philo *philo)
{
	ft_set_goal_achieved(philo);
	if (philo->meal_goal_achieved)
		return (0);
	ft_set_dead_status(philo);
	if (philo->dead)
		return (0);
	return (1);
}
