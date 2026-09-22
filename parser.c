/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 03:04:15 by mbahri            #+#    #+#             */
/*   Updated: 2026/09/01 00:32:06 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_number(char *str, long *value)
{
	long	result;
	int		i;
	int		digit;

	result = 0;
	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		digit = str[i] - '0';
		if (result > (LONG_MAX - digit) / 10)
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*value = result;
	return (1);
}

int	parse_scheduler(char *str, t_scheduler *value)
{
	if (strcmp(str, "fifo") == 0)
	{
		*value = FIFO;
		return (1);
	}
	if (strcmp(str, "edf") == 0)
	{
		*value = EDF;
		return (1);
	}
	return (0);
}

int	parse_args(int argc, char **argv, t_config *config)
{
	long	value;

	if (argc != 9)
		return (0);
	if (!parse_number(argv[1], &value) || value > INT_MAX || value == 0)
		return (0);
	config->number_of_coders = (int)value;
	if (!parse_number(argv[2], &config->time_to_burnout))
		return (0);
	if (!parse_number(argv[3], &config->time_to_compile))
		return (0);
	if (!parse_number(argv[4], &config->time_to_debug))
		return (0);
	if (!parse_number(argv[5], &config->time_to_refactor))
		return (0);
	if (!parse_number(argv[6], &value))
		return (0);
	if (value > INT_MAX || value <= 0)
		return (0);
	config->number_of_compiles_required = (int)value;
	if (!parse_number(argv[7], &config->dongle_cooldown))
		return (0);
	if (!parse_scheduler(argv[8], &config->scheduler))
		return (0);
	return (1);
}
