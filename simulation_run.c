/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.run.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 06:16:27 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 06:44:27 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_start_times(t_simulation *sim)
{
	int	i;

	sim->start_time = get_time_ms();
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		sim->coders[i].last_compile_start = sim->start_time;
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
}

static int	create_coder_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (i);
		i++;
	}
	return (-1);
}

void	wait_for_start(t_simulation *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	while (!sim->started && !sim->stop)
		pthread_cond_wait(&sim->stop_condition,
			&sim->state_mutex);
	pthread_mutex_unlock(&sim->state_mutex);
}

static void	stop_created_threads(t_simulation *sim, int count)
{
	int	i;

	pthread_mutex_lock(&sim->state_mutex);
	sim->stop = 1;
	pthread_cond_broadcast(&sim->stop_condition);
	pthread_mutex_unlock(&sim->state_mutex);
	i = 0;
	while (i < count)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

int	start_simulation(t_simulation *sim)
{
	int	failed;
	int	i;

	failed = create_coder_threads(sim);
	if (failed != -1)
		return (stop_created_threads(sim, failed), 0);
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
		return (stop_created_threads(sim,
				sim->config.number_of_coders), 0);
	init_start_times(sim);
	pthread_mutex_lock(&sim->state_mutex);
	sim->started = 1;
	pthread_cond_broadcast(&sim->stop_condition);
	pthread_mutex_unlock(&sim->state_mutex);
	i = 0;
	while (i < sim->config.number_of_coders)
		pthread_join(sim->coders[i++].thread, NULL);
	pthread_join(sim->monitor_thread, NULL);
	return (1);
}
