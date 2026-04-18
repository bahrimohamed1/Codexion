/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 18:18:52 by username          #+#    #+#             */
/*   Updated: 2026/04/18 23:30:23 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>

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

typedef struct s_dongle
{
	int	id;

}	t_dongle;

typedef struct s_coder
{
	int				id;
	t_simulation	*sim;
	t_dongle		*left_d;
	t_dongle		*right_d;

}	t_coder;

#endif
