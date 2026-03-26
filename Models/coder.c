/* ************************************************************************** */
/*																		      */
/*														:::	  ::::::::        */
/*   coder.c											:+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: afomin  afomin@student.42kl.edu.my		 +#+  +:+	   +#+		      */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2026/03/15 14:57:45 by afomin			#+#	#+#			          */
/*   Updated: 2026/03/15 14:57:46 by afomin		   ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "../codexion.h"

static void	simple_state(t_coder *coder, t_table *table, t_state state)
{
	if (!table->failed)
	{
		coder->action_time = current_time_ms();
		coder->deadline = coder->action_time + table->time_to_burnout;
		if (state == DEBUGGING)
		{
			add_log(table->logger, "is_debugging", coder->id);
			delay(table->time_to_debug);
		}
		else if (state == REFACTORING)
		{
			add_log(table->logger, "is refactoring", coder->id);
			delay(table->time_to_refactor);
		}
	}
}

static void	compile_state(t_coder *coder, t_table *table)
{
	pthread_mutex_lock(&table->queue->mutex);
	rq_add(table->queue, coder);
	broadcast(table->scheduler_condition, NULL);
	wait(coder->condition, &table->queue->mutex, 0);
	pthread_mutex_lock(&table->dongle_mutex);
	while (!is_failed(table) && table->dongles < 2)
		pthread_cond_wait(&table->condition->cond, &table->dongle_mutex);
	pthread_mutex_unlock(&table->queue->mutex);
	if (is_failed(table))
		pthread_mutex_unlock(&table->dongle_mutex);
	else
	{
		table->dongles -= 2;
		pthread_mutex_unlock(&table->dongle_mutex);
		coder->action_time = current_time_ms();
		coder->deadline = coder->action_time + table->time_to_burnout;
		add_log(table->logger, "is compiling", coder->id);
		delay(table->time_to_compile);
		pthread_mutex_unlock(&table->queue->mutex);
		pthread_create(&coder->delayed, NULL, delayed_dongle_release, table);
	}
	broadcast(table->scheduler_condition, NULL);
}

void	*c_life(void *thread_data)
{
	t_table	*table;
	t_coder	*coder;
	t_byte	compiles_done;

	table = ((t_thread_data *) thread_data)->table;
	coder = table->coders[((t_thread_data *) thread_data)->id];
	free(thread_data);
	coder->action_time = current_time_ms();
	coder->deadline = coder->action_time + table->time_to_burnout;
	compiles_done = 0;
	while (!is_failed(table))
	{
		compile_state(coder, table);
		if (++compiles_done == table->compiles_required || is_failed(table))
			break ;
		simple_state(coder, table, DEBUGGING);
		simple_state(coder, table, REFACTORING);
	}
	coder->finished = 1;
	if (!coder->delayed)
		pthread_join(coder->delayed, NULL);
	pthread_cond_destroy(&coder->condition->cond);
	add_log(table->logger, "Finished", coder->id);
	return (NULL);
}
