/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-masr <nel-masr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 15:47:53 by nel-masr          #+#    #+#             */
/*   Updated: 2021/12/29 14:30:47 by nel-masr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	is_he_full(t_pars *pars, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < pars->nb_philo && pars->max_meals != -1)
	{
		pthread_mutex_lock(&(pars->tlm_sanitizer));
		if (philo[i].nb_meals >= pars->max_meals)
			i++;
		else
		{
			pthread_mutex_unlock(&(pars->tlm_sanitizer));
			break ;
		}
		pthread_mutex_unlock(&(pars->tlm_sanitizer));
	}
	if (i == pars->nb_philo)
	{
		pthread_mutex_lock(&(pars->hunger));
		pars->full_philo = 1;
		pthread_mutex_unlock(&(pars->hunger));
		return (1);
	}
	return (0);
}

/*
 * Boucle infinie avec 2 conditions de sortie :
 * - mort d'un philo (pars->death_flag == 1)
 * - tous les philos ont fini de manger si un nombre max de plats est specifie
 * Mutex specifique pour changer la valeur de pars->death_flag / pars->full_philo
 * pour eviter les data races.
 */

void	is_he_dead(t_pars *pars, t_philo *philo)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < pars->nb_philo && pars->death_flag == 0)
		{
			pthread_mutex_lock(&(pars->tlm_sanitizer));
			if (diff(timestamp(), philo[i].time_last_meal) > pars->time_to_die)
			{
				pthread_mutex_lock(&(pars->death));
				pars->death_flag = 1;
				pthread_mutex_unlock(&(pars->death));
				print_death(pars, philo[i].id, "has died.");
			}
			pthread_mutex_unlock(&(pars->tlm_sanitizer));
			i++;
		}
		if (pars->death_flag == 1)
			return ;
		if (is_he_full(pars, philo))
			return ;
	}
}

void	exit_sim(t_pars *pars)
{
	int	i;

	i = 0;
	while (i < pars->nb_philo)
	{
		pthread_join(pars->philo[i].th, NULL);
		i++;
	}
	i = 0;
	while (i < pars->nb_philo)
	{
		pthread_mutex_destroy(&(pars->forks[i]));
		i++;
	}
	pthread_mutex_destroy(&(pars->write));
	pthread_mutex_destroy(&(pars->tlm_sanitizer));
	pthread_mutex_destroy(&(pars->death));
	pthread_mutex_destroy(&(pars->hunger));
}

/*
 * Main lance chaque thread en fct du nb de philos.
 * Puis boucle dans is_he_dead en attendant la mort d'un philo ou
 * qu'ils finissent de manger.
 */

int	init_sim(t_pars *pars)
{
	int		i;
	t_philo	*philo_dup;

	i = 0;
	philo_dup = pars->philo;
	pars->begin = timestamp();
	while (i < pars->nb_philo)
	{
		if (pthread_create(&(philo_dup[i].th), NULL, routine, &(philo_dup[i])))
			return (1);
		i++;
	}
	is_he_dead(pars, philo_dup);
	exit_sim(pars);
	return (0);
}
