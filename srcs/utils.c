/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-masr <nel-masr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 11:34:39 by nel-masr          #+#    #+#             */
/*   Updated: 2021/12/29 14:48:04 by nel-masr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

/*
 * ft_atoll en fait.
 */

long long	ft_atoi(char *s)
{
	int			i;
	int			neg_ctr;
	long long	result;

	i = 0;
	result = 0;
	neg_ctr = 1;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\n'
		|| s[i] == '\r' || s[i] == '\f')
		i++;
	if (s[i] == '+' && s[i + 1] == '-')
		return (0);
	if (s[i] == '+')
		i++;
	if (s[i] == '-')
	{
		neg_ctr *= -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		result = result * 10 + s[i] - '0';
		i++;
	}
	return (result * neg_ctr);
}

/*
 * Mutex pour eviter que les threads ecrivent en meme temps +
 * check de la mort pour ne rien ecrire si un philo est mort.
 */

void	print_action(t_pars *pars, int id, char *action)
{
	pthread_mutex_lock(&(pars->write));
	pthread_mutex_lock(&(pars->death));
	if (pars->death_flag == 0)
	{
		printf("%lld ", timestamp() - pars->begin);
		printf("%d ", id + 1);
		printf("%s\n", action);
	}
	pthread_mutex_unlock(&(pars->write));
	pthread_mutex_unlock(&(pars->death));
}

/*
 * Same as above mais n'est appelee qu'apres avoir active
 * le flag de mort.
 */

void	print_death(t_pars *pars, int id, char *action)
{
	pthread_mutex_lock(&(pars->write));
	pthread_mutex_lock(&(pars->death));
	if (pars->death_flag == 1)
	{
		printf("%lld ", timestamp() - pars->begin);
		printf("%d ", id + 1);
		printf("%s\n", action);
	}
	pthread_mutex_unlock(&(pars->write));
	pthread_mutex_unlock(&(pars->death));
}

/*
 * Fonction appelee dans notre routine de threads pour sortir si un
 * philo est mort.
 */

int	death_check(t_pars *pars)
{
	pthread_mutex_lock(&(pars->death));
	if (pars->death_flag == 1)
	{
		pthread_mutex_unlock(&(pars->death));
		return (1);
	}
	pthread_mutex_unlock(&(pars->death));
	return (0);
}

/* 
 * Same as above mais pour la satiete.
 */

int	hunger_check(t_pars *pars)
{
	pthread_mutex_lock(&(pars->hunger));
	if (pars->full_philo == 1)
	{
		pthread_mutex_unlock(&(pars->hunger));
		return (1);
	}
	pthread_mutex_unlock(&(pars->hunger));
	return (0);
}
