/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 02:07:16 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/26 01:35:19 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config	config;

	printf("%d\n", parse_args(argc, argv, &config));
	printf("Number of coders: %d\n", config.number_of_coders);
	printf("Time to burnout: %ld\n", config.time_to_burnout);
	printf("Time to compile: %ld\n", config.time_to_compile);
	printf("Time to debug: %ld\n", config.time_to_debug);
	printf("Time to refactor: %ld\n", config.time_to_refactor);
	printf("Number of compiles required: %d\n",
		config.number_of_compiles_required);
	printf("Dongle cooldown: %ld\n", config.dongle_cooldown);
	printf("Scheduler: ");
	if (config.scheduler == 0)
		printf("fifo\n");
	else
		printf("edf\n");
}
