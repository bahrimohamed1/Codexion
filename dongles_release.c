/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_release.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 01:09:02 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/31 01:09:09 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		cooldown;

	left = coder->left_dongle;
	right = coder->right_dongle;
	cooldown = get_time_ms()
		+ coder->simulation->config.dongle_cooldown;
	lock_dongle_pair(left, right);
	left->owner = NULL;
	right->owner = NULL;
	left->cooldown_until = cooldown;
	right->cooldown_until = cooldown;
	unlock_dongle_pair(left, right);
}
