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
			new_node->request_time = current_time_ms();
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

void	rq_pop(t_requestQueue *queue)
{
	t_requestQueueNode	*removed;

	if (queue && queue->head)
	{
		removed = queue->head;
		queue->head = queue->head->next;
		free(removed);
	}
}

void	rq_remove(t_requestQueue *queue, int id)
{
	t_requestQueueNode	*curr;
	t_requestQueueNode	*prev;

	if (queue && queue->head)
	{
		curr = queue->head;
		prev = NULL;
		while (curr)
		{
			if (curr->id == (t_byte)id)
				break ;
			prev = curr;
			curr = curr->next;
		}
		if (curr)
		{
			if (prev)
				prev->next = curr->next;
			else
				queue->head = curr->next;
			free(curr);
		}
	}
}
