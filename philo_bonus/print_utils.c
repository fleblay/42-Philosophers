/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fred <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:56:53 by fred              #+#    #+#             */
/*   Updated: 2022/05/11 15:51:23 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>
#include <philo_bonus.h>
#include <stdio.h>
#include <fcntl.h>

void	ft_safe_print(t_data *data, char *msg, int last_msg)
{
	sem_wait(data->s_print);
	if (!ft_sim_is_over(data))
	{
		printf("%lu %d %s\n",
			ft_get_time() - data->start_time, data->id + 1, msg);
		if (!last_msg)
			sem_post(data->s_print);
		else
		{
			sem_wait(data->s_end_of_termination);
			sem_post(data->s_print);
		}
	}
	else
		sem_post(data->s_print);
}
