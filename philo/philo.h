/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:19:25 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/26 12:48:19 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <sys/time.h>

typedef struct s_data {
	struct s_philo	*philo;
	int				philo_count;
	int				ttd;
	int				tte;
	int				tts;
	int				ntepme;
	pthread_t		*thread;
	pthread_mutex_t *fork;
	pthread_mutex_t server_request;
	pthread_mutex_t server_answer;
	pthread_mutex_t server_com;
	pthread_mutex_t server_available_com;
	pthread_mutex_t server_dead_philo;
	pthread_mutex_t print;
	int				available_com;
	int				request_pending;
	int				request;
	int				answer;
	int				dead_philo;
	int				run;
	struct timeval	time;
}	t_data;	

typedef struct s_philo {
	t_data	*data;
	int		id;
	int		answer;
	int		dead;
}	t_philo;

int	init_data(t_data *data, int ac, char *av[]);

#endif
