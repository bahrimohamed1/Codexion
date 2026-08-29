/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_request.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 00:56:25 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/29 02:27:28 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	find_request(t_heap *heap, t_request *request)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->requests[i] == request)
			return (i);
		i++;
	}
	return (-1);
}

int	heap_remove(t_heap *heap, t_request *request, t_scheduler scheduler)
{
	int	target;
	int	parent;

	target = find_request(heap, request);
	if (target == -1)
		return (0);
	heap->size--;
	if (target == heap->size)
		return (1);
	heap->requests[target] = heap->requests[heap->size];
	if (target > 0)
	{
		parent = (target - 1) / 2;
		if (request_priority(heap->requests[target],
				heap->requests[parent], scheduler))
		{
			sift_up(heap, target, scheduler);
			return (1);
		}
	}
	sift_down(heap, target, scheduler);
	return (1);
}

int	request_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	request;
	t_scheduler	scheduler;

	scheduler = coder->simulation->config.scheduler;
	pthread_mutex_lock(&dongle->mutex);
	prepare_request(coder, dongle, &request);
	if (!heap_push(&dongle->queue, &request, scheduler))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	while (!simulation_stopped(coder->simulation)
		&& !request_ready(dongle, &request))
		wait_for_dongle(dongle, &request);
	if (simulation_stopped(coder->simulation))
	{
		heap_remove(&dongle->queue, &request, scheduler);
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	heap_pop(&dongle->queue, scheduler);
	dongle->owner = coder;
	pthread_mutex_unlock(&dongle->mutex);
	log_state(coder, TAKE_DONGLE);
	return (1);
}
