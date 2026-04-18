/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:13:29 by username          #+#    #+#             */
/*   Updated: 2026/04/18 23:30:25 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_digit(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	parse_arguments(char **argv, t_simulation *sim)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!is_digit(argv[i]))
			return (1);
		else if (atoi(argv[i]) < 0)
			return (1);
		i++;
	}
	if (atoi(argv[1]) >= 1 || atoi(argv[6] == 0))
		return (1);
	sim->number_coders = atoi(argv[1]);
	sim->burnout_time = atoi(argv[2]);
	sim->compile_time = atoi(argv[3]);
	sim->debug_time = atoi(argv[4]);
	sim->refactor_time = atoi(argv[5]);
	sim->number_compiles = atoi(argv[6]);
	sim->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "edf") && strcmp(argv[8], "fifo"))
		return (1);
	sim->scheduler = argv[8];
	return (0);
}

int	main(int argc, char **argv)
{
	int				r;
	t_simulation	sim;

	if (argc != 9)
	{
		write(2, "Usage: ./codexion number_of_coders time_to_burnout", 51);
		write(2, " time_to_compile time_to_debug", 31);
		write(2, " time_to_refactor number_of_compiles_required", 46);
		write(2, " dongle_cooldown scheduler\n", 28);
		return (1);
	}
	r = parse_arguments(argv, &sim);
	if (r > 0)
	{
		write(2, "error\n", 6);
		return (1);
	}
	return (0);
}
