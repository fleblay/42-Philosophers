/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fred <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:54:30 by fred              #+#    #+#             */
/*   Updated: 2022/05/11 15:10:07 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_kill_dead_monitor(t_data *data)
{
	sem_post(data->s_dead_signal);
	ft_count_sem_post(data->s_ack_msg, data->philo_count);
}

void	ft_kill_meal_monitor(t_data *data)
{
	ft_count_sem_post(data->s_meal, data->philo_count);
	ft_count_sem_post(data->s_ack_msg, data->philo_count);
}

void	ft_fake_own_death(t_data *data)
{
	sem_post(data->s_dead_signal);
	//simulating ack for dead philo who returned
	sem_post(data->s_ack_msg);
	sem_wait(data->s_end_of_termination);
	//sem_post(data->s_end_of_termination);
	sem_post(data->s_philo_deamon);
}

void	ft_kill_philo_am_i_dead_monitor(t_data *data)
{
	// specifiq ajout thread
	sem_wait(data->s_self_dead[data->id]);
	//data->start_time = ft_get_time();
	data->start_time = MONITOR_INIT_FAILURE;
	sem_post(data->s_self_dead[data->id]);
}
