/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin <afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	free_coders(t_table *table)
{
	t_byte	i;

	if (table && table->coders)
	{
		i = -1;
		while (++i < table->number_of_coders)
		{
			condition_destroy(table->coders[i]->condition);
			free(table->coders[i]);
		}
		free(table->coders);
		table->coders = NULL;
	}
}

void	free_table(t_table *table)
{
	t_requestQueueNode	*n;
	t_requestQueueNode	*next;

	if (table && table->queue)
	{
		n = table->queue->head;
		while (n)
		{
			next = n->next;
			free(n);
			n = next;
		}
		pthread_mutex_destroy(&table->queue->mutex);
		free(table->queue);
	}
	free_coders(table);
	pthread_mutex_destroy(&table->dongle_mutex);
	condition_destroy(table->condition);
	condition_destroy(table->scheduler_condition);
	free(table);
}

static void	*monitor(void *tbl)
{
	t_table	*table;
	t_coder	**coders;
	t_byte	i;

	table = (t_table *) tbl;
	coders = table->coders;
	add_log(table->logger, "Monitoring...", -1);
	i = 0;
	while (!is_failed(table))
	{
		if (!coders[i]->finished && coders[i]->deadline <= current_time_ms())
		{
			add_log(table->logger, "!!! Coder has burned out !!!\n", -1);
			fail(table);
			broadcast(table->condition, &table->dongle_mutex);
		}
		i = (i + 1) % table->number_of_coders;
		delay(1);
	}
	return (NULL);
}

static void	join_threads(t_table *table, pthread_t m, pthread_t s)
{
	t_byte	i;

	i = -1;
	while (++i < table->number_of_coders)
		if (table->coders[i]->thread)
			pthread_join(table->coders[i]->thread, NULL);
	table->failed = 1;
	broadcast(table->scheduler_condition, NULL);
	pthread_join(m, NULL);
	pthread_join(s, NULL);
}

void	run_codexion(t_table *table)
{
	t_byte			i;
	pthread_t		m;
	pthread_t		s;
	t_thread_data	*data;

	i = -1;
	pthread_create(&s, NULL, scheduler, table);
	pthread_create(&m, NULL, monitor, table);
	delay(50);
	i = -1;
	while (++i < table->number_of_coders)
	{
		data = malloc(sizeof(t_thread_data));
		if (!data)
		{
			add_log(table->logger, "Failed to create thread data", i);
			break ;
		}
		data->id = i;
		data->table = table;
		table->coders[i]->action_time = current_time_ms();
		pthread_create(&table->coders[i]->thread, NULL, c_life, data);
	}
	join_threads(table, m, s);
}
