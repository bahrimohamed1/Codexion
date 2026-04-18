/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 22:56:37 by username          #+#    #+#             */
/*   Updated: 2026/04/18 23:30:21 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>

void	init(void)
{
	t_coder		*coders = malloc(sizeof(t_coder) *coders->sim->number_coders);
	t_dongle	*dongles = malloc(sizeof(t_dongle) *coders->sim->number_coders);
	int			i = 0;

	while (i < coders->sim->number_coders)
	{
		coders[i].id = i + 1;
		i++;
	}
}
/*
array dyal coders
array dyal dongle
==> *number_of_coders
*/
