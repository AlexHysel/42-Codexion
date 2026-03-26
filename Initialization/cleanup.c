/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin  afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 19:42:03 by afomin            #+#    #+#             */
/*   Updated: 2026/03/26 19:42:04 by afomin           ###   ########.fr       */
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
			free(table->coders[i]);
			condition_destroy(table->coders[i]->condition);
		}
		free(table->coders);
		table->coders = NULL;
	}
}

void	cleanup(t_table *table)
{
	if (table)
	{
		pthread_mutex_destroy(&table->dongle_mutex);
		pthread_mutex_destroy(&table->failed_mutex);
		condition_destroy(table->condition);
		if (table->scheduler)
		{
			condition_destroy(table->scheduler->condition);
			pthread_mutex_destroy(&table->scheduler->mutex);
			free(table->scheduler->queue);
		}
		if (table->coders)
			free_coders(table);
		free(table);
	}
}
