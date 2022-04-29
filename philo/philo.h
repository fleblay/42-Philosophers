/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:19:25 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/29 10:45:53 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG
# define DEBUG 0
#endif


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
	int				ttt;
	int				ntepme;
	pthread_t		*thread;
	pthread_mutex_t *fork;
	int				*fork_available;
	pthread_mutex_t meal;
	int				*meal_count;
	int				meal_goal_achieved;
	int				philo_is_dead;
	pthread_mutex_t server_request;
	pthread_mutex_t server_answer;
	pthread_mutex_t server_com;
	pthread_mutex_t server_available_com;
	pthread_mutex_t server_dead_philo;
	pthread_mutex_t print;
	pthread_mutex_t start;
	pthread_mutex_t time_mutex;
	int				available_com;
	int				request_pending;
	int				request;
	int				answer;
	int				dead_philo;
	int				run;
	int				start_time;
	struct timeval	time;
}	t_data;	

typedef struct s_philo {
	t_data	*data;
	int		id;
	int		has_com;
	int		answer;
	int		dead;
	int		meal_goal_achieved;
	int		time_last_eat;
	int		start_eat;
	int		start_sleep;
	int		index_fork1;
	int		index_fork2;
	int		ttd;
	int		tte;
	int		tts;
	int		ttt;
	int		start_time;
	int		philo_count;
	int		current_time;
}	t_philo;

int			init_data(t_data *data, int ac, char *av[]);
int			init_philo(t_data *data);
int			launch_philo(t_data *data);
void		*philo_routine(void *phil);
void		cleanup(t_data *data);
int			safe_print(char *txt, t_philo *philo);
int			safe_print_monitor(char *txt, t_data *data);
int			get_sim_duration(int id, char *txt);
int			self_is_dead(t_philo *philo);
int			someone_is_dead(t_philo *philo);
void		wait_for_com_token(t_philo *philo);
void		grab_com_and_place_request(t_philo *philo);
void		wait_for_answer(t_philo *philo);
void		release_com_token_and_com(t_philo *philo);
void		eat_for_time(t_philo *philo);
void		sleep_for_time(t_philo *philo);
int			monitor_someone_is_dead(t_data *data);
void		get_request(t_data *data);
int			check_available_forks(t_data *data);
void		lock_forks(t_philo *philo);
void		release_forks(t_philo *philo);
int			meal_goal_achieved(t_data *data);
void		print_meal_count(t_philo *philo);
int			get_time(void);

#endif
