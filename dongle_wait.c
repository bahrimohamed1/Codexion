/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 02:20:57 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/29 03:19:47 by mbahri           ###   ########.fr       */
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
