/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 02:04:53 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/26 16:53:25 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles(t_simulation *sim)
{
	int	i;
	int	capacity;

	sim->dongles = malloc(sizeof(t_dongle) * sim->config.number_of_coders);
	if (!sim->dongles)
		return (0);
	if (sim->config.number_of_coders == 1)
		capacity = 1;
	else
		capacity = 2;
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (!init_one_dongle(&sim->dongles[i], capacity))
		{
			cleanup_dongles(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_one_dongle(t_dongle *dongle, int capacity)
{
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&dongle->condition, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	if (!init_heap(&dongle->queue, capacity))
	{
        pthread_cond_destroy(&dongle->condition);
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	dongle->owner = NULL;
	dongle->next_sequence = 0;
	dongle->cooldown_until = 0;
	return (1);
}

void	cleanup_dongles(t_simulation *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		destroy_heap(&sim->dongles[i].queue);
		pthread_cond_destroy(&sim->dongles[i].condition);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}
