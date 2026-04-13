/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin  afomin@student.42kl.edu.my		 +#+  +:+	   +#+		      */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:22:34 by afomin            #+#    #+#             */
/*   Updated: 2026/04/01 14:22:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	dongle_init(t_dongle **dongle)
{
	*dongle = malloc(sizeof(t_dongle));
	if (*dongle)
	{
		(*dongle)->cooldown_end = 0;
		(*dongle)->is_taken = 0;
		(*dongle)->queue = malloc(sizeof(t_requestQueue));
		(*dongle)->queue->head = NULL;
		pthread_mutex_init(&(*dongle)->mutex, NULL);
		condition_init(&(*dongle)->cond);
	}
}

void	dongle_destroy(t_dongle *dongle)
{
	if (dongle)
	{
		pthread_mutex_destroy(&dongle->mutex);
		condition_destroy(dongle->cond);
		if (dongle->queue)
			free(dongle->queue);
		free(dongle);
	}
}

static t_byte	best_id(t_requestQueue *queue, t_schedulerType type)
{
	t_requestQueueNode	*curr;
	t_requestQueueNode	*best;

	if (type == FIFO || !queue->head)
		return (queue->head->id);
	curr = queue->head;
	best = curr;
	while (curr)
	{
		if (curr->deadline < best->deadline)
			best = curr;
		curr = curr->next;
	}
	return (best->id);
}

static void	queue(t_dongle *dongle, t_table *table, t_coder *coder)
{
	t_byte	id;

	id = -1;
	if (dongle->is_taken || (dongle->queue && dongle->queue->head))
	{
		rq_add(dongle->queue, coder);
		while (id != coder->id)
		{
			wait(dongle->cond, &dongle->mutex, 0);
			id = best_id(dongle->queue, table->type);
		}
		rq_remove(dongle->queue, id);
	}
}

void	take_dongles(t_coder *coder, t_table *table)
{
	t_dongle	*dongle[2];
	t_byte		i;

	dongle[coder->id % 2] = coder->right_dongle;
	dongle[(coder->id + 1) % 2] = coder->left_dongle;
	i = -1;
	while (++i < 2)
	{
		pthread_mutex_lock(&dongle[i]->mutex);
		queue(dongle[i], table, coder);
		if (dongle[i]->cooldown_end > current_time_ms())
			delay(dongle[i]->cooldown_end - current_time_ms());
		dongle[i]->is_taken = 1;
		pthread_mutex_unlock(&dongle[i]->mutex);
		if (!is_failed(table))
			add_log(table->logger, "has taken a dongle", coder->id);
	}
}
