/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:18:23 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 13:07:53 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_meal_goal_achieved(t_philo *philo)
{
	int	i;

	i = 0;
	if (philo->data->meal_goal == -1)
		return (0);
	while (i < philo->philo_count)
	{
		if (philo->data->meal_count[i] < philo->data->meal_goal)
			return (0);
		i++;
	}
	return (1);
}
