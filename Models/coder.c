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

static void	simple_state(t_coder *coder, t_table *table, char *state)
{
	if (!table->failed)
	{
		add_log(table->logger, state, coder->id);
		coder->action_time = current_time_ms();
		coder->deadline = coder->action_time + table->time_to_burnout;
		if (state[0] == 'd')
			delay(table->time_to_debug);
		else if (state[0] == 'r')
			delay(table->time_to_refactor);
	}
}

static void	compile_state(t_coder *coder, t_table *table)
{
	pthread_mutex_lock(&table->queue->mutex);
	rq_add(table->queue, coder);
	add_log(table->logger, "Request...", coder->id);
	broadcast(table->scheduler_condition, NULL);
	wait(coder->condition, &table->queue->mutex, 0);
	add_log(table->logger, "Waiting for dongles...", coder->id);
	pthread_mutex_lock(&table->dongle_mutex);
	while (!table->failed && table->dongles < 2)
		pthread_cond_wait(&table->condition->cond, &table->dongle_mutex);
	pthread_mutex_unlock(&table->queue->mutex);
	if (!table->failed)
	{
		table->dongles -= 2;
		pthread_mutex_unlock(&table->dongle_mutex);
		add_log(table->logger, "Dongles available.", coder->id);
		coder->action_time = current_time_ms();
		coder->deadline = coder->action_time + table->time_to_burnout;
		add_log(table->logger, "Compiling...", coder->id);
		delay(table->time_to_compile);
		pthread_mutex_lock(&table->dongle_mutex);
		table->dongles += 2;
		broadcast(table->condition, NULL);
		broadcast(table->scheduler_condition, &table->dongle_mutex);
	}
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
	while (!table->failed)
	{
		compile_state(coder, table);
		if (++compiles_done == table->compiles_required || table->failed)
			break ;
		simple_state(coder, table, "debugging");
		simple_state(coder, table, "refactoring");
	}
	coder->finished = 1;
	add_log(table->logger, "Finished coding!", coder->id);
	return (NULL);
}
