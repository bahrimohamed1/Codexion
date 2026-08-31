/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 22:01:13 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 08:33:36 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup_init(t_simulation *sim, int stage)
{
	if (stage >= 4)
		cleanup_dongles(sim, sim->config.number_of_coders);
	if (stage >= 3)
		pthread_mutex_destroy(&sim->log_mutex);
	if (stage >= 2)
		pthread_cond_destroy(&sim->stop_condition);
	if (stage >= 1)
		pthread_mutex_destroy(&sim->state_mutex);
}

int	init_simulation(t_simulation *sim, t_config *config)
{
	sim->config = *config;
	sim->stop = 0;
	sim->start_time = 0;
	sim->started = 0;
	sim->next_sequence = 0;
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&sim->stop_condition, NULL) != 0)
		return (cleanup_init(sim, 1), 0);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (cleanup_init(sim, 2), 0);
	if (!init_dongles(sim))
		return (cleanup_init(sim, 3), 0);
	if (!init_coders(sim))
		return (cleanup_init(sim, 4), 0);
	return (1);
}

void	destroy_simulation(t_simulation *sim)
{
	cleanup_coders(sim, sim->config.number_of_coders);
	cleanup_dongles(sim, sim->config.number_of_coders);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_cond_destroy(&sim->stop_condition);
	pthread_mutex_destroy(&sim->state_mutex);
}

int	simulation_stopped(t_simulation *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->state_mutex);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stopped);
}

void	sim_sleep(t_simulation *sim, long duration)
{
	struct timespec	timeout;
	long			end;

	end = get_time_ms() + duration;
	ms_to_timespec(end, &timeout);
	pthread_mutex_lock(&sim->state_mutex);
	while (!sim->stop && get_time_ms() < end)
		pthread_cond_timedwait(&sim->stop_condition,
			&sim->state_mutex, &timeout);
	pthread_mutex_unlock(&sim->state_mutex);
}
