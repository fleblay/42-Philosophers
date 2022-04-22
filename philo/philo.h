/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:19:25 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/22 17:04:45 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>

typedef struct s_data {
	pthread_t	thread[3];
	pthread_mutex_t fork1;
	pthread_mutex_t fork2;
	pthread_mutex_t fork3;
	pthread_mutex_t server_request;
	pthread_mutex_t server_answer;
	pthread_mutex_t server_com;
	pthread_mutex_t print;
	int	request;
	int	request_pending;
	int	answer;
	int	id;
}	t_data;	

typedef struct s_philo {
	t_data	*data;
	int		id;
	int		answer;
}	t_philo;

#endif
