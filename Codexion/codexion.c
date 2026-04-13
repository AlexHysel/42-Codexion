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

static void	*monitor(void *tbl)
{
	t_table	*table;
	t_coder	**coders;
	t_byte	i;

	table = (t_table *) tbl;
	coders = table->coders;
	i = 0;
	while (!is_failed(table))
	{
		if (is_dead(coders[i]))
		{
			fail(table);
			add_log(table->logger, "burned out", i);
		}
		i = (i + 1) % table->number_of_coders;
		delay(1);
	}
	return (NULL);
}

static void	join_threads(t_table *table, pthread_t m)
{
	t_byte	i;

	i = -1;
	while (++i < table->number_of_coders)
		if (table->coders[i]->thread)
			pthread_join(table->coders[i]->thread, NULL);
	fail(table);
	pthread_join(m, NULL);
}

void	run_codexion(t_table *table)
{
	t_byte			i;
	pthread_t		m;
	t_thread_data	*data;

	pthread_create(&m, NULL, monitor, table);
	i = -1;
	while (++i < table->number_of_coders)
	{
		data = malloc(sizeof(t_thread_data));
		if (!data)
			break ;
		data->coder = table->coders[i];
		data->table = table;
		table->coders[i]->last_compile = current_time_ms();
		pthread_create(&table->coders[i]->thread, NULL, c_life, data);
	}
	join_threads(table, m);
}
