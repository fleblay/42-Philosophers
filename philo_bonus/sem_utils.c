/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fred <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 13:00:08 by fred              #+#    #+#             */
/*   Updated: 2022/05/11 14:55:18 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_count_sem_post(sem_t *sem, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		sem_post(sem);
		i++;
	}
}

void	ft_count_sem_wait(sem_t *sem, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		sem_wait(sem);
		i++;
	}
}

void	ft_unlock_solo_philo(t_data *data)
{
	if (data->philo_count == 1)
	{
		sem_post(data->s_fork);
		sem_post(data->s_print);
		//	printf("fake fork\n");
	}
}

