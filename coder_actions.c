/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 03:54:05 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 04:48:57 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compile_phase(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->simulation;
	if (simulation_stopped(sim))
	{
		release_dongles(coder);
		return (0);
	}
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	log_state(coder, COMPILING);
	sim_sleep(sim, sim->config.time_to_compile);
	release_dongles(coder);
	if (simulation_stopped(sim))
		return (0);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	return (1);
}

int	debug_phase(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->simulation;
	if (simulation_stopped(sim))
		return (0);
	log_state(coder, DEBUGGING);
	sim_sleep(sim, sim->config.time_to_debug);
	if (simulation_stopped(sim))
		return (0);
	return (1);
}

int	refactor_phase(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->simulation;
	if (simulation_stopped(sim))
		return (0);
	log_state(coder, REFACTORING);
	sim_sleep(sim, sim->config.time_to_refactor);
	if (simulation_stopped(sim))
		return (0);
	return (1);
}
