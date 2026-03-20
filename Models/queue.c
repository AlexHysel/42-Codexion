/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin  afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	rq_add(t_requestQueue *queue, t_coder *coder)
{
	t_requestQueueNode	*new_node;
	t_requestQueueNode	*curr;

	if (queue && coder)
	{
		new_node = malloc(sizeof(t_requestQueueNode));
		if (new_node)
		{
			new_node->id = coder->id;
			new_node->request_time = current_time_msec();
			new_node->deadline = coder->action_time + 0ULL;
			new_node->next = NULL;
			pthread_mutex_lock(&queue->mutex);
			curr = queue->head;
			if (curr)
			{
				while (curr->next)
					curr = curr->next;
				curr->next = new_node;
			}
			else
				queue->head = new_node;
			pthread_mutex_unlock(&queue->mutex);
		}
	}
}

void	rq_pop(t_requestQueue *queue)
{
	t_requestQueueNode	*removed;

	if (queue)
	{
		pthread_mutex_lock(&queue->mutex);
		if (queue->head)
		{
			removed = queue->head;
			queue->head = queue->head->next;
			free(removed);
		}
		pthread_mutex_unlock(&queue->mutex);
	}
}

void	rq_add_unsafe(t_requestQueue *queue, t_coder *coder)
{
	t_requestQueueNode	*new_node;
	t_requestQueueNode	*curr;

	if (queue && coder)
	{
		new_node = malloc(sizeof(t_requestQueueNode));
		if (new_node)
		{
			new_node->id = coder->id;
			new_node->request_time = current_time_msec();
			new_node->deadline = coder->action_time + 0ULL;
			new_node->next = NULL;
			curr = queue->head;
			if (curr)
			{
				while (curr->next)
					curr = curr->next;
				curr->next = new_node;
			}
			else
				queue->head = new_node;
		}
	}
}

void	rq_pop_unsafe(t_requestQueue *queue)
{
	t_requestQueueNode	*removed;

	if (queue && queue->head)
	{
		removed = queue->head;
		queue->head = queue->head->next;
		free(removed);
	}
}
