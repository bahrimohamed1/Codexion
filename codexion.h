/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbahri <mbahri@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 21:40:28 by mbahri            #+#    #+#             */
/*   Updated: 2026/08/27 19:42:13 by mbahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <limits.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>

typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;

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
	t_config		config;
	long			start_time;
	int				stop;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor_thread;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	log_mutex;
}	t_simulation;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long			last_compile_start;
	int				compile_count;
	pthread_mutex_t	state_mutex;
	t_simulation	*simulation;
}	t_coder;

typedef struct s_request
{
	t_coder			*coder;
	long			deadline;
	unsigned long	sequence;
}	t_request;

typedef struct s_heap
{
	t_request	**requests;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	condition;
	t_coder			*owner;
	long			cooldown_until;
	t_heap			queue;
	unsigned long	next_sequence;
}	t_dongle;

typedef enum e_state
{
	TAKE_DONGLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
}	t_state;

int			parse_number(char *str, long *value);
int			parse_scheduler(char *str, t_scheduler *value);
int			parse_args(int argc, char **argv, t_config *config);
long		get_time_ms(void);
long		get_elapsed_time(long start_time);
int			init_heap(t_heap *queue, int capacity);
void		destroy_heap(t_heap *heap);
int			init_dongles(t_simulation *sim);
int			init_one_dongle(t_dongle *dongle, int capacity);
void		cleanup_dongles(t_simulation *sim, int count);
int			init_coders(t_simulation *sim);
int			init_one_coder(t_simulation *sim, int num_coders, int i);
void		cleanup_coders(t_simulation *sim, int count);
int			init_simulation(t_simulation *sim, t_config *config);
void		destroy_simulation(t_simulation *sim);
void		log_state(t_coder *coder, t_state state);
void		log_burnout(t_coder *coder);
int			request_priority(t_request *a, t_request *b, t_scheduler scheduler);
int			heap_push(t_heap *heap, t_request *request, t_scheduler scheduler);
t_request	*heap_peek(t_heap *heap);

#endif
