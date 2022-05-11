/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:08:57 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/11 15:30:33 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_set_philo(t_data *data, t_philo *philo, int i)
{
	philo->data = data;
	philo->id = i + 1;
	philo->dead = 0;
	philo->philo_count = data->philo_count;
	philo->can_eat = 0;
	philo->last_start_eat = 0;
	philo->meal_goal_achieved = 0;
	philo->ttd = data->ttd;
	philo->tte = data->tte;
	philo->tts = data->tts;
	philo->left_fork = philo->id - 1 ;
	philo->right_fork = philo->id % philo->philo_count ;
}

static int	ft_launch_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		ft_set_philo(data, &data->philo[i], i);
		if (pthread_create(&data->thread[i], NULL,
				philo_routine, &data->philo[i]))
			return (0);
		i += 2;
	}
	i = 1;
	while (i < data->philo_count)
	{
		ft_set_philo(data, &data->philo[i], i);
		if (pthread_create(&data->thread[i], NULL,
				philo_routine, &data->philo[i]))
			return (0);
		i += 2;
	}
	return (0);
	return (1);
}

int	ft_launch_philo(t_data *data)
{
	pthread_mutex_lock(&data->m_start);
	if (!ft_launch_threads(data))
	{
		data->start_time = PHILO_CREATE_FAILURE;
		return (ft_putstr_fd("Error : creating threads\n", 2), 0);
	}
	else
		data->start_time = ft_get_time();
	return (1);
}

void	ft_join_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->thread[i], NULL);
		i++;
	}
}
