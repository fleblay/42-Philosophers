/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:01:51 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 15:48:13 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stddef.h>
#include <unistd.h>

void	*philo_routine(void *philo_struct)
{
	t_philo	*philo;

	philo = (t_philo *)philo_struct;
	ft_set_start_time(philo);
	if (philo->start_time == 0)
		return (NULL);
	if (philo->id % 2 == 0)
		usleep((philo->tte - 10) * 1000);
	while (philo->dead == 0)
	{
		if (ft_preliminary_checks(philo) == 0)
			break ;
		ft_check_if_can_eat(philo);
		if (philo->can_eat == 1)
		{
			if (!ft_grab_forks(philo) || !ft_eat(philo)
				|| !ft_sleep(philo) || !ft_think(philo))
				break ;
		}
		else
			usleep(100);
	}
	return (NULL);
}
