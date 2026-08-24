/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 21:40:28 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/23 17:44:01 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <limits.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_config
{
	int			number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	int			number_of_compiles_required;
	long		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_simulation
{
	t_config		*config;
	long			start_time;
	int				stop;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor_thread;
	pthread_mutex_t	state_mutex;	// for considiration
	pthread_mutex_t	log_mutex;
}	t_simulation;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*ight_dongle;
	long			last_compile_start;
	int				compile_count;
	pthread_mutex_t	state_mutex;	// to protect last_c_c and compile_count
	t_simulation	*simulation;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;

}	t_dongle;

int		parse_number(char *str, long *value);
int		parse_scheduler(char *str, t_scheduler *value);
int		parse_args(int argc, char **argv, t_config *config);
long	get_elapsed_time(long start_time);

#endif
