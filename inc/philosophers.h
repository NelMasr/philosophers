/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-masr <nel-masr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:22:17 by nel-masr          #+#    #+#             */
/*   Updated: 2021/12/29 14:25:16 by nel-masr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_pars;

typedef struct s_philo
{
	int				id;
	int				right_fork;
	int				left_fork;
	struct s_pars	*pars;
	int				nb_meals;
	long long		time_last_meal;
	pthread_t		th;
}					t_philo;

typedef struct s_pars
{
	long long		nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				full_philo;
	long long		begin;
	int				death_flag;
	pthread_mutex_t	forks[300];
	pthread_mutex_t	write;
	pthread_mutex_t	tlm_sanitizer;
	pthread_mutex_t	death;
	pthread_mutex_t	hunger;
	t_philo			philo[300];
}				t_pars;

/*
**	MAIN
*/

int			main(int ac, char **av);
int			check_parsing(t_pars *pars, char **av);
int			ft_is_numeric(char **av);

/*
**	PARSING
*/

int			init_parsing(t_pars *pars, char **av);
int			init_mutex(t_pars *pars);
int			init_philo(t_pars *pars);

/*
** SIM
*/

int			init_sim(t_pars *pars);
void		exit_sim(t_pars *pars);
void		is_he_dead(t_pars *pars, t_philo *philo);
int			is_he_full(t_pars *pars, t_philo *philo);

/*
** ROUTINE
*/

void		*routine(void *philo_dup);
void		eating_routine(t_philo *philo);
void		take_forks(t_pars *pars, t_philo *philo);

/*
** UTILS
*/

long long	ft_atoi(char *s);
void		print_action(t_pars *pars, int id, char *action);
int			death_check(t_pars *pars);
int			hunger_check(t_pars *pars);
void		print_death(t_pars *pars, int id, char *action);

/*
** TIME
*/

long long	timestamp(void);
void		custom_sleep(t_pars *pars, long long sleep);
long long	diff(long long a, long long b);

#endif
