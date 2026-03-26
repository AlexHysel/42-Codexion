/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin afomin@student.42kl.edu.my          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:03:15 by afomin            #+#    #+#             */
/*   Updated: 2026/03/23 14:03:17 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static t_logNode	*pop_log(t_logger *logger)
{
	t_logNode	*node;

	node = NULL;
	if (logger)
	{
		pthread_mutex_lock(&logger->mutex);
		node = logger->log_list;
		if (node)
			logger->log_list = node->next;
		pthread_mutex_unlock(&logger->mutex);
		if (node)
			node->next = NULL;
	}
	return (node);
}

static void	*log(void *data)
{
	t_logNode		*log_data;
	t_logger		*logger;

	logger = (t_logger *)data;
	while (!logger->finished)
	{
		wait(logger->condition, &logger->mutex, 1);
		pthread_mutex_unlock(&logger->mutex);
		log_data = pop_log(logger);
		while (log_data && !logger->finished)
		{
			printf("%llu ", log_data->time);
			if (log_data->id != 255)
				printf("%d ", log_data->id);
			printf("%s\n", log_data->msg);
			free(log_data);
			log_data = pop_log(logger);
		}
	}
	return (NULL);
}

t_logger	*run_logger(void)
{
	t_logger	*logger;

	logger = malloc(sizeof(t_logger));
	if (logger)
	{
		logger->log_list = NULL;
		logger->start_time = current_time_ms();
		logger->finished = 0;
		pthread_mutex_init(&logger->mutex, NULL);
		condition_init(&logger->condition);
		if (pthread_create(&logger->thread, NULL, log, logger))
		{
			pthread_mutex_destroy(&logger->mutex);
			condition_destroy(logger->condition);
			free(logger);
			return (NULL);
		}
	}
	return (logger);
}

void	stop_logger(t_logger *logger)
{
	t_logNode	*current;
	t_logNode	*next;

	pthread_mutex_lock(&logger->mutex);
	current = logger->log_list;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	logger->finished = 1;
	broadcast(logger->condition, &logger->mutex);
	pthread_join(logger->thread, NULL);
	pthread_mutex_destroy(&logger->mutex);
	condition_destroy(logger->condition);
	free(logger);
}

void	add_log(t_logger *logger, char *msg, t_byte id)
{
	t_logNode	*log_data;
	t_logNode	*current;

	if (!logger)
		return ;
	log_data = malloc(sizeof(t_logNode));
	if (!log_data)
		return ;
	log_data->id = id;
	log_data->msg = msg;
	log_data->time = current_time_ms() - logger->start_time;
	log_data->next = NULL;
	pthread_mutex_lock(&logger->mutex);
	if (!logger->log_list)
		logger->log_list = log_data;
	else
	{
		current = logger->log_list;
		while (current->next)
			current = current->next;
		current->next = log_data;
	}
	broadcast(logger->condition, &logger->mutex);
}
