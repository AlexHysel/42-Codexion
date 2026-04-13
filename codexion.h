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
#define SUCCESS 0
#define FAILURE 1

/*
⢸⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⡷⠀⠀
⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀
⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇ Are ya winning son?
⢸⠀⠀⠀⠀⠀⠖⠒⠒⠒⢤⠀⠀⠀⡇
⢸⠀⠀⣀⢤⣼⣀⡠⠤⠤⠼⠤⡄⠀⡇⠀No, im redoing the project for the... 6th time?
⢸⠀⠀⠑⡤⠤⡒⠒⠒⡊⠙⡏⠀⢀⡇⠀Some day i will learn how to pay more attention to the subject.
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
typedef unsigned char		t_byte;

typedef enum e_scheduler
{
	EDF,
	FIFO
}	t_schedulerType;

typedef struct s_condition
{
	pthread_cond_t	cond;
	t_byte			flag;
}	t_condition;

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

typedef struct s_dongle
{
	t_msec			cooldown_end;
	t_byte			is_taken;
	t_mutex			mutex;
	t_requestQueue	*queue;
	t_condition		*cond;
}	t_dongle;

typedef struct s_coder
{
	t_uint		id;
	pthread_t	thread;
	t_msec		last_compile;
	t_msec		deadline;
	t_mutex		mutex;
	t_byte		finished;
	t_dongle	*right_dongle;
	t_dongle	*left_dongle;
}	t_coder;

typedef struct s_logNode
{
	t_byte				id;
	char				*msg;
	t_msec				time;
	struct s_logNode	*next;
}	t_logNode;

typedef struct s_logger
{
	t_mutex		mutex;
	t_condition	*condition;
	t_msec		start_time;
	t_logNode	*log_list;
	t_byte		finished;
	pthread_t	thread;
}	t_logger;

typedef struct s_table
{
	t_uint			number_of_coders;
	t_msec			time_to_burnout;
	t_msec			time_to_compile;
	t_msec			time_to_debug;
	t_msec			time_to_refactor;
	t_uint			compiles_required;
	t_msec			dongle_cooldown;
	t_schedulerType	type;

	t_condition		*condition;
	t_byte			failed;
	t_mutex			mutex;
	t_logger		*logger;
	t_coder			**coders;
}	t_table;

typedef struct s_thread_data
{
	t_table	*table;
	t_coder	*coder;
}	t_thread_data;

// ===== Initialization =====
int			validate_args(char **args);
t_table		*setup_codexion(char **args);

// ===== Codexion =====
void		run_codexion(t_table *table);
void		cleanup(t_table *table);

// ===== Models =====
void		*c_life(void *thread_data);
t_byte		is_dead(t_coder *coder);

void		wait(t_condition *cond, t_mutex *mutex, t_byte lock);
void		broadcast(t_condition *condition, t_mutex *mutex);
void		condition_init(t_condition **condition);
void		condition_destroy(t_condition *condition);

void		dongle_init(t_dongle **dongle);
void		dongle_destroy(t_dongle *dongle);
void		take_dongles(t_coder *coder, t_table *table);

void		rq_add(t_requestQueue *queue, t_coder *coder);
void		rq_pop(t_requestQueue *queue);
void		rq_remove(t_requestQueue *queue, t_byte id);

// ===== Utils =====
t_msec		current_time_ms(void);
void		delay(t_msec milliseconds);
void		fail(t_table *table);
t_byte		is_failed(t_table *table);

void		add_log(t_logger *logger, char *msg, t_byte id);
t_logger	*run_logger(void);
void		stop_logger(t_logger *logger);