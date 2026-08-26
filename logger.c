/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 19:40:09 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/26 22:44:05 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_coder *coder, t_state state)
{
	t_simulation	*sim;
	long			time;
	int				should_log;

	should_log = 0;
	sim = coder->simulation;
	pthread_mutex_lock(&sim->state_mutex);
	should_log = !(sim->stop && state != BURNED_OUT);
	pthread_mutex_unlock(&sim->state_mutex);
	if (!should_log)
		return ;
	time = get_elapsed_time(sim->start_time);
	pthread_mutex_lock(&sim->log_mutex);
	printf("%ld %d %s\n", time, coder->id, state_message(state));
	pthread_mutex_unlock(&sim->log_mutex);
}

char	*state_message(t_state state)
{
	if (state == TAKE_DONGLE)
		return ("has taken a dongle");
	if (state == COMPILING)
		return ("is compiling");
	if (state == DEBUGGING)
		return ("is debugging");
	if (state == REFACTORING)
		return ("is refactoring");
	if (state == BURNED_OUT)
		return ("burned out");
	return (NULL);
}
