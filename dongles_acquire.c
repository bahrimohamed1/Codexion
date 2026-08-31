/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_acquire.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 06:50:18 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 01:24:19 by mbahri           ###   ########.fr       */
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

static int	handle_single_coder(t_coder *coder)
{
	t_dongle	*dongle;

	dongle = coder->left_dongle;
	pthread_mutex_lock(&dongle->mutex);
	if (!dongle->owner && get_time_ms() >= dongle->cooldown_until)
	{
		dongle->owner = coder;
		pthread_mutex_unlock(&dongle->mutex);
		log_state(coder, TAKE_DONGLE);
		return (0);
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

static int	try_claim_pair(t_coder *coder, t_request *left_req,
		t_request *right_req)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left_dongle;
	right = coder->right_dongle;
	lock_dongle_pair(left, right);
	if (!pair_ready(coder, left_req, right_req))
	{
		unlock_dongle_pair(left, right);
		return (0);
	}
	claim_pair(coder);
	unlock_dongle_pair(left, right);
	log_dongle_pair(coder);
	return (1);
}

int	acquire_dongles(t_coder *coder)
{
	t_request	left_req;
	t_request	right_req;

	if (coder->left_dongle == coder->right_dongle)
		return (handle_single_coder(coder));
	if (!enqueue_pair(coder, &left_req, &right_req))
		return (0);
	while (!simulation_stopped(coder->simulation))
	{
		if (try_claim_pair(coder, &left_req, &right_req))
			return (1);
		usleep(500);
	}
	remove_pair_requests(coder, &left_req, &right_req);
	return (0);
}
