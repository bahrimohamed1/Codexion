/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 01:19:53 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/26 01:34:39 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_heap(t_heap *heap, int capacity)
{
	if (capacity == 0)
		return (0);
	heap->requests = malloc(sizeof(t_request *) * capacity);
	if (!heap->requests)
		return (0);
	heap->size = 0;
	heap->capacity = capacity;
	return (1);
}

void	destroy_heap(t_heap *heap)
{
	free(heap->requests);
	heap->requests = NULL;
	heap->size = 0;
	heap->capacity = 0;
}
