/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 05:05:21 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 06:48:26 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_burned_out(t_coder *coder, long now)
{
	long	deadline;
	int		finished;

	pthread_mutex_lock(&coder->state_mutex);
	finished = (coder->compile_count
			>= coder->simulation->config.number_of_compiles_required);
	deadline = coder->last_compile_start
		+ coder->simulation->config.time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
	if (finished)
		return (0);
	return (now >= deadline);
}

static int	all_coders_finished(t_simulation *sim)
{
	int	i;
	int	finished;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		finished = (sim->coders[i].compile_count
				>= sim->config.number_of_compiles_required);
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		if (!finished)
			return (0);
		i++;
	}
	return (1);
}

static void	wake_stopped_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	pthread_cond_broadcast(&sim->stop_condition);
	pthread_mutex_unlock(&sim->state_mutex);
}

static void	stop_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->stop = 1;
	pthread_cond_broadcast(&sim->stop_condition);
	pthread_mutex_unlock(&sim->state_mutex);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				i;
	long			now;

	sim = (t_simulation *)arg;
	wait_for_start(sim);
	if (simulation_stopped(sim))
		return (NULL);
	while (!simulation_stopped(sim))
	{
		now = get_time_ms();
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			if (coder_burned_out(&sim->coders[i], now))
				return (log_burnout(&sim->coders[i]),
					wake_stopped_simulation(sim), NULL);
			i++;
		}
		if (all_coders_finished(sim))
			return (stop_simulation(sim), NULL);
		usleep(500);
	}
	return (NULL);
}
