/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_acquire.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 06:50:18 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/29 08:34:58 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	pair_ready(t_coder *coder, t_request *left_req,
	t_request	*right_req)
{
	t_dongle	*left;
	t_dongle	*right;
	long		now;

	left = coder->left_dongle;
	right = coder->right_dongle;
	now = get_time_ms();
	if (heap_peek(&left->queue) != left_req)
		return (0);
	if (heap_peek(&right->queue) != right_req)
		return (0);
	if (left->owner != NULL || right->owner != NULL)
		return (0);
	if (now < left->cooldown_until || now < right->cooldown_until)
		return (0);
	return (1);
}

static void	claim_pair(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	t_scheduler	scheduler;

	left = coder->left_dongle;
	right = coder->right_dongle;
	scheduler = coder->simulation->config.scheduler;
	heap_pop(&left->queue, scheduler);
	heap_pop(&right->queue, scheduler);
	left->owner = coder;
	right->owner = coder;
}

static void	remove_pair_requests(t_coder *coder, t_request *left_req,
		t_request *right_req)
{
	t_dongle	*left;
	t_dongle	*right;
	t_scheduler	scheduler;

	left = coder->left_dongle;
	right = coder->right_dongle;
	scheduler = coder->simulation->config.scheduler;
	lock_dongle_pair(left, right);
	heap_remove(&left->queue, left_req, scheduler);
	heap_remove(&right->queue, right_req, scheduler);
	unlock_dongle_pair(left, right);
}
