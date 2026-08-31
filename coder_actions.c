/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 03:54:05 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 22:09:19 by mbahri           ###   ########.fr       */
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

int	coder_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->state_mutex);
	finished = (coder->compile_count
			>= coder->simulation->config.number_of_compiles_required);
	pthread_mutex_unlock(&coder->state_mutex);
	return (finished);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	wait_for_start(coder->simulation);
	if (simulation_stopped(coder->simulation))
		return (NULL);
	if (coder->id % 2 == 0
		|| (coder->simulation->config.number_of_coders % 2 != 0
			&& coder->id == coder->simulation->config.number_of_coders))
		usleep(1000);
	while (!simulation_stopped(coder->simulation))
	{
		if (!acquire_dongles(coder))
			break ;
		if (!compile_phase(coder))
			break ;
		if (!debug_phase(coder))
			break ;
		if (!refactor_phase(coder))
			break ;
		if (coder_finished(coder))
			break ;
	}
	return (NULL);
}
