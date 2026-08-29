/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:37:47 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/28 03:00:16 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_priority(t_request *a, t_request *b, t_scheduler scheduler)
{
	if (scheduler == FIFO)
		return (a->sequence < b->sequence);
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	return (a->sequence < b->sequence);
}

int	heap_push(t_heap *heap, t_request *request, t_scheduler scheduler)
{
	int	i;

	if (heap->size >= heap->capacity)
		return (0);
	i = heap->size;
	heap->requests[i] = request;
	heap->size++;
	sift_up(heap, i, scheduler);
	return (1);
}

t_request	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (heap->requests[0]);
}
