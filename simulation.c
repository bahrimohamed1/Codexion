/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 22:01:13 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/26 02:07:40 by mbahri           ###   ########.fr       */
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
		return (0);
	if (!init_coders(sim))
	{
		pthread_mutex_destroy(&sim->state_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
		return (0);
	}
	return (1);
}

int	init_coders(t_simulation *sim)
{
	int	i;
	
	sim->coders = malloc(sizeof(t_coder) * sim->config.number_of_coders);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].compile_count = 0;
		sim->coders[i].id = i;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].left_dongle = 
	}
	
}
