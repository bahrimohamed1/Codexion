/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_request.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 00:56:25 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 08:42:48 by mbahri           ###   ########.fr       */
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

int	enqueue_pair(t_coder *coder, t_request *left_req,
		t_request *right_req)
{
	t_dongle		*left;
	t_dongle		*right;
	t_scheduler		scheduler;
	unsigned long	sequence;

	left = coder->left_dongle;
	right = coder->right_dongle;
	scheduler = coder->simulation->config.scheduler;
	pthread_mutex_lock(&coder->simulation->state_mutex);
	sequence = coder->simulation->next_sequence++;
	pthread_mutex_unlock(&coder->simulation->state_mutex);
	lock_dongle_pair(left, right);
	prepare_request(coder, left_req, sequence);
	prepare_request(coder, right_req, sequence);
	if (!heap_push(&left->queue, left_req, scheduler))
		return (unlock_dongle_pair(left, right), 0);
	if (!heap_push(&right->queue, right_req, scheduler))
	{
		heap_remove(&left->queue, left_req, scheduler);
		return (unlock_dongle_pair(left, right), 0);
	}
	unlock_dongle_pair(left, right);
	return (1);
}

void	lock_dongle_pair(t_dongle *a, t_dongle *b)
{
	if (a == b)
	{
		pthread_mutex_lock(&a->mutex);
		return ;
	}
	if (a < b)
	{
		pthread_mutex_lock(&a->mutex);
		pthread_mutex_lock(&b->mutex);
	}
	else
	{
		pthread_mutex_lock(&b->mutex);
		pthread_mutex_lock(&a->mutex);
	}
}

void	unlock_dongle_pair(t_dongle *a, t_dongle *b)
{
	if (a == b)
	{
		pthread_mutex_unlock(&a->mutex);
		return ;
	}
	if (a < b)
	{
		pthread_mutex_unlock(&b->mutex);
		pthread_mutex_unlock(&a->mutex);
	}
	else
	{
		pthread_mutex_unlock(&a->mutex);
		pthread_mutex_unlock(&b->mutex);
	}
}
