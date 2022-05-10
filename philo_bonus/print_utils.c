/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fred <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:56:53 by fred              #+#    #+#             */
/*   Updated: 2022/05/10 11:12:09 by fred             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>
#include <philo_bonus.h>
#include <stdio.h>
#include <fcntl.h>

void	ft_safe_print(t_data* data, char *msg, int last_msg)
{
	//printf("trying to print\n");
	sem_wait(data->s_print);
	//printf("finished waiting for print semaphore\n");
	check_if_end_sim(data);
	if (data->go_on)
	{
		//printf("data go_on is true\n");
		printf("%lu %d %s\n", ft_get_time() - data->start_time, data->id + 1, msg);
		if (!last_msg)
			sem_post(data->s_print);
		else
		{
		//	printf("waiting for end of termination\n");
			sem_wait(data->s_end_of_termination);
		//	printf("waiting for end of termination OVER\n");
			ft_count_sem_post(data->s_print, data->philo_count - 1);
		}
	}
	else
	{
		//printf("data go_on is false, returning\n");
	}
}

/*
void	ft_safe_print(t_data* data, char *msg, int last_msg)
{
	sem_t	*last_print;

	printf("trying to print\n");
	sem_wait(data->s_print);
	last_print = sem_open("/s_last_print", 0);
	if (last_print != SEM_FAILED)
	{
		sem_close(last_print);
		sem_post(data->s_print);
		return ;
	}
	printf("%lu %d %s\n", ft_get_time() - data->start_time, data->id, msg);
	if (last_msg)
	{
		last_print = sem_open("/s_last_print", O_CREAT, 0644, 0);
		sem_close(last_print);
		printf("this is last msg\n");
	}
	sem_post(data->s_print);
}
*/
