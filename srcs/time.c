/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-masr <nel-masr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 11:29:20 by nel-masr          #+#    #+#             */
/*   Updated: 2021/12/29 14:45:34 by nel-masr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

/*
 * Pointe le temps pour une action donnee. gettimeofday() donne le nombre de
 * secondes et microsecondes ecoulees depuis le 01/01/1970, valeurs qu'il
 * faut convertir en millisecondes et additionner.
 */

long long	timestamp(void)
{
	struct timeval	t;
	long long		result;

	gettimeofday(&t, NULL);
	result = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	return (result);
}

/*
 * usleep suspend l'execution du thread appelant pendant AU MOINS x micro-
 * secondes. Donc si on l'appelle de maniere brute notre programme peut
 * ralentir + que prevu et ainsi entrainer la mort d'un philo. Du coup on
 * fait une boucle infinie ou on appelle plusieurs fois usleep pendant une
 * tres petite duree et une fois que le temps est ecoule est >= a notre tte/
 * tts on sort de la boucle.
 */

void	custom_sleep(t_pars *pars, long long sleep)
{
	long long	i;

	i = timestamp();
	while (1)
	{
		pthread_mutex_lock(&(pars->death));
		if (pars->death_flag == 1)
		{
			pthread_mutex_unlock(&(pars->death));
			break ;
		}
		pthread_mutex_unlock(&(pars->death));
		if (diff(timestamp(), i) >= sleep)
			break ;
		usleep(10);
	}
}

long long	diff(long long a, long long b)
{
	return (a - b);
}
