/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin <afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static t_byte	get_id_by_time(t_table *table)
{
	t_byte	id;
	t_byte	i;
	t_msec	b_delta;
	t_msec	delta;

	i = -1;
	id = -1;
	b_delta = -1;
	while (++i < table->number_of_coders)
	{
		if (table->coders[i]->scheduler_requested)
		{
			delta = c_delta(table, i);
			if (b_delta > delta)
			{
				id = i;
				b_delta = delta;
			}
		}
	}
	return (id);
}

static void	next_id_edf(t_table *table)
{
	t_byte	id;

	id = get_id_by_time(table);
	if (id != 255
		&& (!table->queue || id != table->queue->id))
	{
		if (!table->queue)
		{
			table->queue = malloc(sizeof(t_requestQueue));
			if (table->queue)
			{
				table->queue->id = id;
				table->queue->next = NULL;
			}
			else
				logger(table, "next_id_edf: Allocation error");
		}
		else
			table->queue->id = id;
		log_coder(table, "Is next for compilation", id);
	}
	else
		logger(table, "No coders are waiting for compilation.");
}

static void	next_id_fifo(t_table *table, t_byte id)
{
	q_add_unsafe(table, id);
	log_coder(table, "Added to queue.", id);
}

void	*scheduler(void *data)
{
	t_table	*table;
	t_byte	id;

	table = (t_table *) data;
	id = 2;
	logger(table, "Scheduler launched...");
	while (!table->failed)
	{
		pthread_mutex_lock(&table->queue_mutex);
		pthread_cond_wait(&table->scheduler_condition, &table->queue_mutex);
		logger(table, "Updating queue...");
		if (table->scheduler == EDF)
			next_id_edf(table);
		else
			next_id_fifo(table, id);
		if (table->queue)
			table->coders[table->queue->id]->scheduler_requested = 0;
		pthread_mutex_unlock(&table->queue_mutex);
		pthread_cond_broadcast(&table->condition);
	}
	return (NULL);
}
