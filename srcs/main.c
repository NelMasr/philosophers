/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-masr <nel-masr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:40:05 by nel-masr          #+#    #+#             */
/*   Updated: 2021/12/29 14:12:16 by nel-masr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	ft_is_numeric(char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (av[i])
	{
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (3);
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}

int	check_parsing(t_pars *pars, char **av)
{
	int	ret;

	ret = ft_is_numeric(av);
	if (ret == 3)
	{
		printf("Error: only whole positive numbers are allowed.\n");
		return (1);
	}
	ret = init_parsing(pars, av);
	if (ret == 1)
	{
		printf("Invalid arguments. Requirements : nb_philo >= 1 || <= 200 ");
		printf("| time_to_x >= 0 || < 2000\n");
		return (1);
	}
	if (ret == 2)
	{
		printf("Error initializing mutex or philo struct.\n");
		return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_pars	pars;

	if (ac != 5 && ac != 6)
	{
		printf("Usage: ./philosophers number_of_philosophers time_to_die ");
		printf("time_to_eat time_to_sleep [must_eat]\n");
		return (1);
	}
	if (check_parsing(&pars, av) == 1)
		return (1);
	if (init_sim(&pars) == 1)
	{
		printf("Problem initiating sim.\n");
		return (1);
	}
	return (0);
}
