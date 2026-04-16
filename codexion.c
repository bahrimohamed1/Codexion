/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:13:29 by mbahri            #+#    #+#             */
/*   Updated: 2026/04/16 14:48:26 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_digit(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int parse_arguments(int argc, char **argv, t_simulation *sim)
{
	if (argc == 9)
	{
		int	i;

		i = 0;
		while	(argv[i])
		{
			if (!is_digit(argv[i]))
				return (1);

			i++;
		}
			return 1;
		int number_coders = atoi(argv[1]);
		int	burnout_time = atoi(argv[2]);
		int compile_time = atoi(argv[3]);
		int debug_time = atoi(argv[4]);
		int refactor_time = atoi(argv[5]);
		int number_compiles = atoi(argv[6]);
		int dongle_cooldown = atoi(argv[7]);
		char *scheduler = argv[8];
	}

	printf("Usage: ./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler\n")
}
int	main(int argc, char *argv[])
{
	parse_arguments(argc, argv, sim);

	return 1;
}
