/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin afomin@student.42kl.edu.my          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#define EDF_STR "edf"
#define FIFO_STR "fifo"

/*
⢸⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⡷⠀⠀
⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀
⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇ Are ya winning son?
⢸⠀⠀⠀⠀⠀⠖⠒⠒⠒⢤⠀⠀⠀⡇
⢸⠀⠀⣀⢤⣼⣀⡠⠤⠤⠼⠤⡄⠀⡇⠀
⢸⠀⠀⠑⡤⠤⡒⠒⠒⡊⠙⡏⠀⢀⡇⠀
⢸⠀⠀⠀⠇⠀⣀⣀⣀⣀⢀⠧⠟⠁⡇
⢸⠀⠀⠀⠸⣀⠀⠀⠈⢉⠟⠓⠀⠀⡇
⢸⠀⠀⠀⠀⠈⢱⡖⠋⠁⠀⠀⠀⠀⡇
⢸⠀⠀⠀⠀⣠⢺⠧⢄⣀⠀⠀⣀⣀⡇
⢸⠀⠀⠀⣠⠃⢸⠀⠀⠈⠉⡽⠿⠯⡆
⢸⠀⠀⣰⠁⠀⢸⠀⠀⠀⠀⠉⠉⠉⡇
⢸⠀⠀⠣⠀⠀⢸⢄⠀⠀⠀⠀⠀⠀⡇
⢸⠀⠀⠀⠀⠀⢸⠀⢇⠀⠀⠀⠀⠀⡇ 
*/

typedef unsigned int		t_uint;
typedef unsigned long long	t_msec;
typedef pthread_mutex_t		t_mutex;
typedef pthread_cond_t		t_condition;
typedef unsigned char		t_byte;

typedef enum e_scheduler
{
	EDF,
	FIFO
}	t_schedulerType;

typedef struct s_coder
{
	t_uint		id;
	pthread_t	thread;
	t_msec		action_time;
	t_msec		deadline;
	t_condition	condition;
	t_byte		finished;
}	t_coder;

typedef struct s_queue_node
{
	t_byte				id;
	t_msec				request_time;
	t_msec				deadline;
	struct s_queue_node	*next;
}	t_requestQueueNode;

typedef struct s_queue
{
	t_mutex				mutex;
	t_requestQueueNode	*head;
}	t_requestQueue;

typedef struct s_table
{
	t_uint			number_of_coders;
	t_msec			time_to_burnout;
	t_msec			time_to_compile;
	t_msec			time_to_debug;
	t_msec			time_to_refactor;
	t_uint			compiles_required;
	t_msec			dongle_cooldown;
	t_schedulerType	scheduler;

	t_mutex			dongle_mutex;
	t_requestQueue	*queue;
	t_condition		condition;
	t_condition		scheduler_condition;
	t_uint			dongles;
	t_byte			failed;
	t_msec			start_time;
	t_coder			**coders;
}	t_table;

typedef struct s_thread_data
{
	t_table	*table;
	t_byte	id;
}	t_thread_data;

// ===== Initialization =====
int		validate_args(char **args);
int		app_run(int argc, char **args);

// ===== Codexion =====
void	run_codexion(t_table *table);
void	*scheduler(void *data);

// ===== Models =====
void	*c_life(void *thread_data);

void	rq_add(t_requestQueue *queue, t_coder *coder);
void	rq_add_unsafe(t_requestQueue *queue, t_coder *coder);
void	rq_pop(t_requestQueue *queue);
void	rq_pop_unsafe(t_requestQueue *queue);
void	rq_remove_unsafe(t_requestQueue *queue, int id);

t_byte	t_get_dongles(t_table *table);

// ===== Utils =====
t_msec	current_time_ms(void);
void	delay(t_msec milliseconds);
void	logger(char *msg, t_byte id, t_msec start_time);
int		error(char *msg);
