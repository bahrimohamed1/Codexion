/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 02:20:57 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 07:02:44 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	prepare_request(t_coder *coder, t_dongle *dongle,
	t_request	*request)
{
	long	time_to_burnout;

	time_to_burnout = coder->simulation->config.time_to_burnout;
	request->coder = coder;
	pthread_mutex_lock(&coder->state_mutex);
	request->deadline = coder->last_compile_start + time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
	request->sequence = dongle->next_sequence++;
}

void	remove_pair_requests(t_coder *coder, t_request *left_req,
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
