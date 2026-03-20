/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin  afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:17:42 by afomin            #+#    #+#             */
/*   Updated: 2026/03/16 15:17:43 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

t_byte	t_get_dongles(t_table *table)
{
	t_byte	dongles;

	pthread_mutex_lock(&table->dongle_mutex);
	dongles = table->dongles;
	pthread_mutex_unlock(&table->dongle_mutex);
	return (dongles);
}
