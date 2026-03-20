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
		log_coder(table, state, coder->id);
		c_update_action_time(table, coder->id);
		if (state[0] == 'd')
			delay(table->time_to_debug);
		else if (state[0] == 'r')
			delay(table->time_to_refactor);
	}
}

static void	compile_state(t_coder *coder, t_table *table)
{
	rq_add(table->queue, coder);
	pthread_mutex_lock(&table->dongle_mutex);
	while (!table->failed && table->dongles < 2)
		pthread_cond_wait(&table->condition, &table->dongle_mutex);
	pthread_cond_broadcast(&table->scheduler_condition);
	pthread_lock_mutex(&table->queue->mutex);
	while (!table->failed)
		pthread_cond_wait(&coder->condition, &table->dongle_mutex);
	if (!table->failed)
	{
		q_pop(table);
		pthread_mutex_unlock(&table->dongle_mutex);
		table->dongles -= 2;
		log_coder(table, "Compiling...", coder->id);
		delay(table->time_to_compile);
		pthread_mutex_lock(&table->dongle_mutex);
		table->dongles += 2;
	}
	pthread_cond_broadcast(&table->condition);
	pthread_mutex_unlock(&table->dongle_mutex);
}

void	*c_life(void *thread_data)
{
	t_table	*table;
	t_coder	*coder;
	t_byte	compiles_done;

	table = ((t_thread_data *) thread_data)->table;
	coder = table->coders[((t_thread_data *) thread_data)->id];
	compiles_done = 0;
	while (!table->failed)
	{
		compile_state(coder, table);
		if (++compiles_done == table->compiles_required || table->failed)
			break ;
		simple_state(coder, table, "debugging");
		simple_state(coder, table, "refactoring");
	}
	free(thread_data);
	return (NULL);
}
