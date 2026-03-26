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
	t_byte			id;
	t_requestQueue	*queue;

	queue = table->scheduler->queue;
	broadcast(table->condition, NULL);
	if (table && queue && queue->head)
	{
		while (queue->head)
		{
			id = queue->head->id;
			rq_pop(queue);
			broadcast(table->coders[id]->condition, NULL);
		}
	}
}

void	finish_scheduler(t_scheduler *scheduler)
{
	pthread_mutex_lock(&scheduler->mutex);
	scheduler->finished = 1;
	broadcast(scheduler->condition, &scheduler->mutex);
}

static t_byte	finished(t_scheduler *scheduler)
{
	t_byte	status;

	pthread_mutex_lock(&scheduler->mutex);
	status = scheduler->finished;
	pthread_mutex_unlock(&scheduler->mutex);
	return (status);
}

void	*scheduler(void *data)
{
	t_table		*t;
	t_scheduler	*s;
	t_byte		id;

	t = (t_table *) data;
	s = t->scheduler;
	while (!finished(s))
	{
		wait(s->condition, &s->queue->mutex, 1);
		if (is_failed(t))
			release_all_coders(t);
		else if (s->queue && s->queue->head)
		{
			if (s->type == EDF)
				id = get_id_by_time(s->queue);
			else
				id = s->queue->head->id;
			rq_remove(s->queue, id);
			broadcast(t->coders[id]->condition, NULL);
		}
		pthread_mutex_unlock(&s->queue->mutex);
	}
	return (NULL);
}
