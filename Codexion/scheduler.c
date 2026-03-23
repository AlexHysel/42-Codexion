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

static t_byte	get_id_by_time(t_requestQueue *queue)
{
	t_requestQueueNode	*curr;
	t_byte				id;
	t_msec				best_deadline;

	id = -1;
	if (queue && queue->head)
	{
		curr = queue->head;
		best_deadline = -1;
		while (curr)
		{
			if (curr->deadline < best_deadline)
			{
				best_deadline = curr->deadline;
				id = curr->id;
			}
			curr = curr->next;
		}
	}
	return (id);
}

void	*scheduler(void *data)
{
	t_table	*table;
	t_byte	id;

	table = (t_table *) data;
	add_log(table->logger, "Scheduler launched...", -1);
	while (!table->failed)
	{
		pthread_mutex_lock(&table->queue->mutex);
		pthread_cond_wait(&table->scheduler_condition, &table->queue->mutex);
		if (!table->failed && table->scheduler == EDF)
		{
			id = get_id_by_time(table->queue);
			if (id != 255)
			{
				rq_remove(table->queue, id);
				pthread_cond_broadcast(&table->coders[id]->condition);
				add_log(table->logger, "Is next in queue", id);
			}
			else
				add_log(table->logger, "Request Queue is empty", -1);
		}
		else if (!table->failed)
		{
			if (table->queue && table->queue->head)
			{
				id = table->queue->head->id;
				rq_pop(table->queue);
				pthread_cond_broadcast(&table->coders[id]->condition);
				add_log(table->logger, "Is next in queue", id);
			}
			else
				add_log(table->logger, "Request Queue is empty", -1);
		}
		pthread_mutex_unlock(&table->queue->mutex);
	}
	return (NULL);
}
