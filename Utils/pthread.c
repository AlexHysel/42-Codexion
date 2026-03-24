/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin <afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 15:18:21 by afomin            #+#    #+#             */
/*   Updated: 2026/03/24 15:18:22 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	condition_init(t_condition **condition)
{
	*condition = malloc(sizeof(t_condition));
	if (*condition)
	{
		pthread_cond_init(&(*condition)->cond, NULL);
		(*condition)->flag = 0;
	}
}

void	wait(t_condition *condition, t_mutex *mutex, t_byte lock)
{
	if (lock)
		pthread_mutex_lock(mutex);
	condition->flag = 0;
	while (!condition->flag)
		pthread_cond_wait(&condition->cond, mutex);
}

void	broadcast(t_condition *condition, t_mutex *mutex)
{
	condition->flag = 1;
	pthread_cond_broadcast(&condition->cond);
	if (mutex)
		pthread_mutex_unlock(mutex);
}

void	condition_destroy(t_condition *condition)
{
	if (condition)
	{
		pthread_cond_destroy(&condition->cond);
		free(condition);
	}
}
