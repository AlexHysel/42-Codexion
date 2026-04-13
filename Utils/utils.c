/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin afomin@student.42kl.edu.my          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 18:36:37 by afomin            #+#    #+#             */
/*   Updated: 2026/03/12 18:36:38 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

t_msec	current_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return ((t_msec)tv.tv_sec * 1000 + (t_msec)tv.tv_usec / 1000);
}

void	delay(t_msec milliseconds)
{
	usleep(milliseconds * 1000);
}

t_byte	is_failed(t_table *table)
{
	t_byte	failed;

	pthread_mutex_lock(&table->mutex);
	failed = table->failed;
	pthread_mutex_unlock(&table->mutex);
	return (failed);
}

void	fail(t_table *table)
{
	pthread_mutex_lock(&table->mutex);
	table->failed = 1;
	pthread_mutex_unlock(&table->mutex);
}
