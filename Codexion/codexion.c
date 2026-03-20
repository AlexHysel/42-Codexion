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
	while (!table->failed)
	{
		if (c_delta(table, i) <= 0)
		{
			table->failed = 1;
			logger(table, "!!! Coder has burned out !!!\n");
			pthread_cond_broadcast(&table->condition);
		}
		i = (i + 1) % table->number_of_coders;
		delay(1);
	}
	return (NULL);
}

void	run_codexion(t_table *table)
{
	t_byte			i;
	pthread_t		m;
	pthread_t		s;
	t_thread_data	*data;

	i = -1;
	while (++i < table->number_of_coders)
		table->coders[i]->action_time = time_ms();
	i = -1;
	if (table->scheduler == EDF)
		pthread_create(&s, NULL, scheduler, table);
	pthread_create(&m, NULL, monitor, table);
	logger(table, "Monitoring...");
	while (++i < table->number_of_coders)
	{
		data = malloc(sizeof(t_thread_data));
		data->id = i;
		data->table = table;
		pthread_create(&table->coders[i]->thread, NULL, c_life, data);
	}
	i = -1;
	while (++i < table->number_of_coders)
		pthread_join(table->coders[i]->thread, NULL);
	pthread_join(m, NULL);
	pthread_join(s, NULL);
}
