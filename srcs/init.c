/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-masr <nel-masr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 10:52:17 by nel-masr          #+#    #+#             */
/*   Updated: 2021/12/29 14:49:31 by nel-masr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	init_mutex(t_pars *pars)
{
	int	i;

	i = 0;
	while (i < pars->nb_philo)
	{
		if (pthread_mutex_init(&(pars->forks[i]), NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&(pars->write), NULL))
		return (1);
	if (pthread_mutex_init(&(pars->tlm_sanitizer), NULL))
		return (1);
	if (pthread_mutex_init(&(pars->death), NULL))
		return (1);
	if (pthread_mutex_init(&(pars->hunger), NULL))
		return (1);
	return (0);
}

/*
 * Formule pour right fork est pour s'assurer que les philos soient bien
 * assis en cercle. (faire un dessin)
 */

int	init_philo(t_pars *pars)
{
	int	i;

	i = 0;
	while (i < pars->nb_philo)
	{
		pars->philo[i].id = i;
		pars->philo[i].left_fork = i;
		pars->philo[i].right_fork = (i + 1) % pars->nb_philo;
		pars->philo[i].pars = pars;
		pars->philo[i].time_last_meal = timestamp();
		pars->philo[i].nb_meals = 0;
		i++;
	}
	return (0);
}

int	init_parsing(t_pars *pars, char **av)
{
	pars->nb_philo = ft_atoi(av[1]);
	pars->time_to_die = ft_atoi(av[2]);
	pars->time_to_eat = ft_atoi(av[3]);
	pars->time_to_sleep = ft_atoi(av[4]);
	if (pars->nb_philo < 1 || pars->time_to_die <= 0 || pars->time_to_eat < 0
		|| pars->time_to_sleep < 0 || pars->nb_philo > 2147483647
		|| pars->time_to_die > 2147483647 || pars->time_to_eat > 2147483647
		|| pars->time_to_sleep > 2147483647)
		return (1);
	if (av[5])
	{
		pars->max_meals = ft_atoi(av[5]);
		if (pars->max_meals <= 0)
			return (1);
	}
	else
		pars->max_meals = -1;
	pars->death_flag = 0;
	pars->full_philo = 0;
	if (init_mutex(pars) == 1 || init_philo(pars) == 1)
		return (2);
	return (0);
}
