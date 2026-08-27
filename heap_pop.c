/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_pop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:36:06 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/27 19:59:20 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	heap_swap(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_down(t_heap *heap, int index, t_scheduler scheduler)
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
		heap_swap(heap->requests[index], heap->requests[best]);
		return ;
	}
}
