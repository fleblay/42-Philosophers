/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sem_open.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fred <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 12:50:02 by fred              #+#    #+#             */
/*   Updated: 2022/05/11 12:56:39 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <fcntl.h>

char *ft_create_self_dead_sem_name(int i)
{
	char	*name;

	name = (char *)malloc(13 * sizeof(char));
	if (!name)
		return (NULL);
	ft_strlcpy(name, "/s_philo_000", 13);
	name[9] = i / 100 + 48; 
	name[10] = i / 10 - (i / 100) * 10 + 48; 
	name[11] = i - (i / 10) * 10 - (i / 100) * 100 + 48;
	return (name);
}

int	ft_safe_sem_open(sem_t **sem_ptr, char *sem_name, int value)
{
	*sem_ptr = sem_open(sem_name, O_CREAT, 0644, value);
	if (*sem_ptr == SEM_FAILED)
		return (0);
	return (1);
}

int ft_create_sem(t_data *data)
{
	if (!ft_safe_sem_open(&data->s_print, "/s_print", 1))
		return (ft_sem_destroy(data, SELF_DEAD), 0);
	if (!ft_safe_sem_open(&data->s_start, "/s_start", 0))
		return (ft_sem_destroy(data, SELF_DEAD | PRINT), 0);
	if (!ft_safe_sem_open(&data->s_end_simu, "/s_end_simu", 0))
		return (ft_sem_destroy(data, SELF_DEAD | PRINT | START), 0);
	if (!ft_safe_sem_open(&data->s_meal, "/s_meal", 0))
		return (ft_sem_destroy(data, SELF_DEAD | PRINT | START | END_SIM), 0);
	if (!ft_safe_sem_open(&data->s_fork, "/s_fork", data->philo_count))
		return (ft_sem_destroy(data, SELF_DEAD | PRINT | START | END_SIM
		| MEAL), 0);
	if (!ft_safe_sem_open(&data->s_dead_signal, "/s_dead_signal", 0))
		return (ft_sem_destroy(data, SELF_DEAD | PRINT | START | END_SIM
				| MEAL | FORK), 0);
	if (!ft_safe_sem_open(&data->s_ack_msg, "/s_ack_msg", 0))
		return (ft_sem_destroy(data, SELF_DEAD | PRINT | START | END_SIM
				| MEAL | FORK | DEAD_SIGN), 0);
	if (!ft_safe_sem_open(&data->s_philo_deamon, "/s_philo_deamon", 0))
		return (ft_sem_destroy(data, SELF_DEAD | PRINT | START | END_SIM
				| MEAL | FORK | DEAD_SIGN | ACK_MSG), 0);
	if (!ft_safe_sem_open(&data->s_end_of_termination, "/s_eot", 0))
		return (ft_sem_destroy(data, SELF_DEAD | PRINT | START | END_SIM
				| MEAL | FORK | DEAD_SIGN | ACK_MSG | DEAMON), 0);
	return (1);
}

int	ft_create_sem_tab(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->s_self_dead[i] = sem_open(data->self_dead_name[i],
			O_CREAT, 0644, 1);
		if (data->s_self_dead[i] == SEM_FAILED)
		{
			while (--i >= 0)
			{
				sem_close(data->s_self_dead[i]);
				sem_unlink(data->self_dead_name[i]);
			}
			return (0);
		}
		i++;
	}
	return (1);
}
