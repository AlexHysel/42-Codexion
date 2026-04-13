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
			pthread_mutex_destroy(&table->coders[i]->mutex);
			dongle_destroy(table->coders[i]->right_dongle);
			table->coders[i]->right_dongle = NULL;
			table->coders[i]->left_dongle = NULL;
			free(table->coders[i]);
		}
		free(table->coders);
		table->coders = NULL;
	}
}

void	cleanup(t_table *table)
{
	if (table)
	{
		pthread_mutex_destroy(&table->mutex);
		condition_destroy(table->condition);
		if (table->coders)
			free_coders(table);
		free(table);
	}
}
