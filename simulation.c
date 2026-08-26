/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 22:01:13 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/26 21:58:43 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_simulation(t_simulation *sim, t_config *config)
{
	sim->config = *config;
	sim->stop = 0;
	sim->start_time = 0;
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->state_mutex);
		return (0);
	}
	if (!init_dongles(sim))
	{
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->state_mutex);
		return (0);
	}
	if (!init_coders(sim))
	{
		cleanup_dongles(sim, sim->config.number_of_coders);
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->state_mutex);
		return (0);
	}
	return (1);
}

void	destroy_simulation(t_simulation *sim)
{
	cleanup_coders(sim, sim->config.number_of_coders);
	cleanup_dongles(sim, sim->config.number_of_coders);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->state_mutex);
}
