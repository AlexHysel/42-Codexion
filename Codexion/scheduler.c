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
	t_table	*t;
	t_byte	id;

	t = (t_table *) data;
	add_log(t->logger, "Scheduler launched...", -1);
	while (!is_failed(t))
	{
		wait(t->scheduler_condition, &t->queue->mutex, 1);
		if (is_failed(t) && t->scheduler == EDF && t->queue && t->queue->head)
		{
			id = get_id_by_time(t->queue);
			rq_remove(t->queue, id);
			broadcast(t->coders[id]->condition, NULL);
			add_log(t->logger, "Is next in queue", id);
		}
		else if (!is_failed(t) && t->queue && t->queue->head)
		{
			id = t->queue->head->id;
			rq_pop(t->queue);
			broadcast(t->coders[id]->condition, NULL);
			add_log(t->logger, "Is next in queue", id);
		}
		pthread_mutex_unlock(&t->queue->mutex);
	}
	add_log(t->logger, "Scheduler finished.", -1);
	return (NULL);
}
