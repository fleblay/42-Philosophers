/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:19:25 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 13:16:02 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# ifndef DEBUG
#  define DEBUG 0
# endif

# include <pthread.h>

//flags for ft_mutex_destroy

# define FORK_TAB 1<<0
# define PRINT 1<<1
# define START 1<<2
# define TIME 1<<3
# define DEAD 1<<4
# define CHECK_FORK 1<<5
# define MEAL 1<<6
# define ALL FORK_TAB | PRINT | START | TIME | DEAD | CHECK_FORK | MEAL

typedef struct s_data {
	struct s_philo	*philo;
	int				philo_count;
	unsigned long	ttd;
	unsigned long	tte;
	unsigned long	tts;
	int				dead_philo;
	unsigned long	start_time;
	int				meal_goal;
	pthread_t		*thread;
	pthread_mutex_t	*m_fork;
	int				*fork_available;
	int				*meal_count;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_start;
	pthread_mutex_t	m_time;
	pthread_mutex_t	m_dead;
	pthread_mutex_t	m_check_fork;
	pthread_mutex_t	m_meal;
}	t_data;	

typedef struct s_philo {
	t_data			*data;
	int				id;
	int				dead;
	int				philo_count;
	int				meal_goal_achieved;
	int				can_eat;
	int				last_start_eat;
	int				last_start_sleep;
	unsigned int	left_fork;
	unsigned int	right_fork;
	unsigned long	ttd;
	unsigned long	tte;
	unsigned long	tts;
	unsigned long	start_time;
	unsigned long	current_time;
}	t_philo;

long	ft_satoi(const char *nptr, int *error);
void	ft_putstr_fd(char *s, int fd);
int		ft_init_data(t_data *data, int ac, char *av[]);
void	ft_deallocate(t_data *data);
void	ft_mutex_destroy(t_data *data, int flags);
int		ft_launch_philo(t_data *data);
void	*philo_routine(void *philo_struct);
unsigned long	ft_get_time(void);
void	ft_join_philo(t_data *data);
void	s_print(t_philo *philo, char *msg, int dead);
int		ft_someone_is_dead(t_philo *philo);
int	ft_eat_for_time(t_philo *philo);
int	ft_sleep_for_time(t_philo *philo);
void	ft_release_forks(t_philo *philo);
int	ft_meal_goal_achieved(t_philo *philo);
int	ft_self_is_dead(t_philo *philo);
#endif
