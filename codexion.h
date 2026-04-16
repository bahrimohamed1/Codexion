/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.h                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/04/16 18:18:52 by username         #+#    #+#              */
/*   Updated: 2026/04/16 18:27:56 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

typedef struct s_simulation
{
	int		number_coders;
	int		burnout_time;
	char	*scheduler;
	int		compile_time;
	int		debug_time;
	int		refactor_time;
	int		number_compiles;
	int		dongle_cooldown;
}	t_simulation;

#endif
