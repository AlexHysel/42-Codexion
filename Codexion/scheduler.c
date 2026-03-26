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

static void	release_all_coders(t_table *table)
{
	t_byte	id;

	broadcast(table->condition, NULL);
	if (table && table->queue && table->queue->head)
	{
		while (table->queue->head)
		{
			id = table->queue->head->id;
			add_log(table->logger, "Returned", id);
			rq_pop(table->queue);
			broadcast(table->coders[id]->condition, NULL);
		}
	}
}

void	finish_sceduler(t_table *table)
{
	pthread_mutex_lock(&table->scheduler_mutex);
	table->scheduler_finish = 1;
	broadcast(table->scheduler_condition, &table->scheduler_mutex);
}

static t_byte	finished(t_table *table)
{
	t_byte	status;

	pthread_mutex_lock(&table->scheduler_mutex);
	status = table->scheduler_finish;
	pthread_mutex_unlock(&table->scheduler_mutex);
	return (status);
}

void	*scheduler(void *data)
{
	t_table	*t;
	t_byte	id;

	t = (t_table *) data;
	while (!finished(t))
	{
		wait(t->scheduler_condition, &t->queue->mutex, 1);
		if (is_failed(t))
			release_all_coders(t);
		else if (t->scheduler == EDF && t->queue && t->queue->head)
		{
			id = get_id_by_time(t->queue);
			rq_remove(t->queue, id);
			broadcast(t->coders[id]->condition, NULL);
		}
		else if (t->queue && t->queue->head)
		{
			id = t->queue->head->id;
			rq_pop(t->queue);
			broadcast(t->coders[id]->condition, NULL);
		}
		pthread_mutex_unlock(&t->queue->mutex);
	}
	add_log(t->logger, "Scheduler finished", -1);
	return (NULL);
}
