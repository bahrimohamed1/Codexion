/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_pop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:36:06 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/28 03:00:00 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_request	*heap_pop(t_heap *heap, t_scheduler scheduler)
{
	t_request	*root;

	if (heap->size == 0)
		return (NULL);
	root = heap->requests[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->requests[0] = heap->requests[heap->size];
		sift_down(heap, 0, scheduler);
	}
	return (root);
}
