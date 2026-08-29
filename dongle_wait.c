/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 02:20:57 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/29 02:31:55 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_stopped(t_simulation *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->state_mutex);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stopped);
}

void	prepare_request(t_coder *coder, t_dongle *dongle,
	t_request	*request)
{
	long	time_to_burnout;

	time_to_burnout = coder->simulation->config.time_to_burnout;
	request->coder = coder;
	pthread_mutex_lock(&coder->state_mutex);
	request->deadline = coder->last_compile_start + time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
	request->sequence = dongle->next_sequence++;
}

int	request_ready(t_dongle *dongle, t_request *request)
{
	if (heap_peek(&dongle->queue) != request)
		return (0);
	if (dongle->owner != NULL)
		return (0);
	if (get_time_ms() < dongle->cooldown_until)
		return (0);
	return (1);
}

void	wait_for_dongle(t_dongle *dongle, t_request *request)
{
	struct timespec	timeout;

	if (heap_peek(&dongle->queue) == request && !dongle->owner)
	{
		ms_to_timespec(dongle->cooldown_until, &timeout);
		pthread_cond_timedwait(&dongle->condition,
			&dongle->mutex, &timeout);
	}
	else
		pthread_cond_wait(&dongle->condition, &dongle->mutex);
}
