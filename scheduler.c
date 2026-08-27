/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:37:47 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/27 19:35:03 by mbahri           ###   ########.fr       */
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

static void	heap_swap(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_heap *heap, int index, t_scheduler scheduler)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_priority(heap->requests[index],
				heap->requests[parent], scheduler))
			break ;
		heap_swap(&heap->requests[index], &heap->requests[parent]);
		index = parent;
	}
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
