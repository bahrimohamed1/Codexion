/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 16:41:08 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/26 17:03:01 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_coders(t_simulation *sim)
{
	int	i;
	int	num_coders;

	num_coders = sim->config.number_of_coders;
	sim->coders = malloc(sizeof(t_coder) * num_coders);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < num_coders)
	{
		if (!init_one_coder(sim, num_coders, i))
		{
			cleanup_coders(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_one_coder(t_simulation *sim, int num_coders, int i)
{
	if (pthread_mutex_init(&sim->coders[i].state_mutex, NULL) != 0)
		return (0);
	sim->coders[i].compile_count = 0;
	sim->coders[i].id = i + 1;
	sim->coders[i].last_compile_start = 0;
	sim->coders[i].left_dongle = &sim->dongles[i];
	sim->coders[i].right_dongle = &sim->dongles[(i + 1) % num_coders];
	sim->coders[i].simulation = sim;
	return (1);
}

void	cleanup_coders(t_simulation *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->coders[i].state_mutex);
		i++;
	}
	free(sim->coders);
	sim->coders = NULL;
}
