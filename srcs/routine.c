/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-masr <nel-masr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 10:49:11 by nel-masr          #+#    #+#             */
/*   Updated: 2021/12/29 14:40:31 by nel-masr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

/*
 * Fonction specifique pour le cas ou on a 1 philo.
 * Dans ce cas, philo->lf == philo->rf, creant des warnings sur valgrind si
 * il essaie de prendre la "meme" fourchette.
 * Apres avoir pris sa fourchette, il attend sa mort via un custom_sleep.
 */

void	one_philo(t_pars *pars, t_philo *philo)
{
	pthread_mutex_lock(&(pars->forks[philo->right_fork]));
	print_action(pars, philo->id, "has taken a fork.");
	pthread_mutex_lock(&(pars->tlm_sanitizer));
	philo->time_last_meal = timestamp();
	pthread_mutex_unlock(&(pars->tlm_sanitizer));
	custom_sleep(pars, pars->time_to_die * 10);
	pthread_mutex_unlock(&(pars->forks[philo->right_fork]));
}

/*
 * Si tous les philos prennent leurs fourchette gauche puis droite, on sera
 * face a un probleme de starvation pour le dernier philo. On lui donne la
 * priorite sur les fourchettes en le faisant commencer par la droite puis
 * la gauche + petit usleep pour pas que les autres philos lui prennent.
 */

void	take_forks(t_pars *pars, t_philo *philo)
{
	if (philo->id == pars->nb_philo - 1)
	{
		pthread_mutex_lock(&(pars->forks[philo->right_fork]));
		print_action(pars, philo->id, "has taken a fork.");
		pthread_mutex_lock(&(pars->forks[philo->left_fork]));
		print_action(pars, philo->id, "has taken a fork.");
	}
	else
	{
		usleep(50);
		pthread_mutex_lock(&(pars->forks[philo->left_fork]));
		print_action(pars, philo->id, "has taken a fork.");
		pthread_mutex_lock(&(pars->forks[philo->right_fork]));
		print_action(pars, philo->id, "has taken a fork.");
	}
}

/*
 * Routine = on prend les fourchettes -> lock le moment du dernier meal ->
 * timestamp -> unlock -> philo mange -> lock nb_meals -> incremente
 * nb_meals -> unlock nb_meals -> unlock les fourchettes.
 */

void	eating_routine(t_philo *philo)
{
	t_pars	*pars;

	pars = philo->pars;
	if (pars->nb_philo == 1)
		one_philo(pars, philo);
	else
	{
		take_forks(pars, philo);
		pthread_mutex_lock(&(pars->tlm_sanitizer));
		print_action(pars, philo->id, "is eating.");
		philo->time_last_meal = timestamp();
		pthread_mutex_unlock(&(pars->tlm_sanitizer));
		custom_sleep(pars, pars->time_to_eat);
		pthread_mutex_lock(&(pars->tlm_sanitizer));
		philo->nb_meals++;
		pthread_mutex_unlock(&(pars->tlm_sanitizer));
		pthread_mutex_unlock(&(pars->forks[philo->left_fork]));
		pthread_mutex_unlock(&(pars->forks[philo->right_fork]));
	}
}

/* On fait dormir les philos pairs (au sens du sujet) pour forcer les
 * impairs a commencer et eviter un deadlock. Ensuite ils mangent,
 * puis dorment, puis reflechissent, en faisant un check de notre
 * flag de death apres chaque operation et un check de satiete apres
 * avoir mange.
 */

void	*routine(void *philo_dup)
{
	t_philo	*philo;
	t_pars	*pars;

	philo = (t_philo *)philo_dup;
	pars = philo->pars;
	if (philo->id % 2 != 0)
		usleep(15000);
	while (1)
	{
		if (death_check(pars) == 1)
			break ;
		eating_routine(philo);
		if (hunger_check(pars) == 1)
			break ;
		if (death_check(pars) == 1)
			break ;
		print_action(pars, philo->id, "is sleeping.");
		custom_sleep(pars, pars->time_to_sleep);
		if (death_check(pars) == 1)
			break ;
		print_action(pars, philo->id, "is thinking.");
	}
	return (NULL);
}
