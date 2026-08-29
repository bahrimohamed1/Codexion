/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 02:59:40 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/28 03:03:32 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_swap(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sift_down(t_heap *heap, int index, t_scheduler scheduler)
{
	int	right;
	int	left;
	int	best;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		best = index;
		if (left < heap->size && request_priority(heap->requests[left],
				heap->requests[best], scheduler))
			best = left;
		if (right < heap->size && request_priority(heap->requests[right],
				heap->requests[best], scheduler))
			best = right;
		if (best == index)
			break ;
		heap_swap(&heap->requests[index], &heap->requests[best]);
		index = best;
	}
}

void	sift_up(t_heap *heap, int index, t_scheduler scheduler)
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
