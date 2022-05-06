/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:19:25 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/06 11:21:18 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# ifndef DEBUG
#  define DEBUG 0
# endif

//flags for ft_destroy_sem

# define PRINT 1
# define START 2
# define END_SIM 4
# define MEAL 8
# define FORK 16
# define DEAD_SIGN 32
# define ACK_MSG 64
# define ALL 127

# include <pthread.h>
# include <semaphore.h>
# include <sys/types.h>
# include <stddef.h>

typedef struct s_data {
	int				philo_count;
	unsigned long	ttd;
	unsigned long	tte;
	unsigned long	tts;
	int				meal_goal;
	sem_t			*s_print;
	sem_t			*s_start;
	sem_t			*s_end_simu;
	sem_t			*s_meal;
	sem_t			*s_fork;
	sem_t			*s_dead_signal;
	sem_t			*s_ack_msg;
	sem_t			**s_self_dead;
	char			**self_dead_name;
	int				dead;
	int				last_start_eat;
	int				last_start_sleep;
	int				last_start_think;
	int				meal_goal_achieved;
	pid_t			*philo_pid;

	unsigned long	start_time;
	unsigned long	current_time;
}	t_data;	

long			ft_satoi(const char *nptr, int *error);
void			ft_putstr_fd(char *s, int fd);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
int				ft_init_data(t_data *data, int ac, char *av[]);
int				ft_get_param(t_data *data, int ac, char *av[]);
int				ft_create_sem(t_data *data);
int				ft_create_sem_tab(t_data *data);
void			ft_sem_destroy(t_data *data, int flags);
void			ft_set_data(t_data *data);
int				ft_allocate(t_data *data);
void			ft_sem_close(t_data *data, int flags);
void			ft_sem_destroy(t_data *data, int flags);
void			ft_sem_unlink(int flags);
void			ft_deallocate(t_data *data);
int				ft_philo_routine(t_data *data, int i);
#endif
