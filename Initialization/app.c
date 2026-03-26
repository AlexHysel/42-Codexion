/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin  afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	create_queue(t_table *table)
{
	table->queue = malloc(sizeof(t_requestQueue));
	if (table->queue)
	{
		table->queue->head = NULL;
		pthread_mutex_init(&table->dongle_mutex, NULL);
		pthread_mutex_init(&table->queue->mutex, NULL);
		condition_init(&table->condition);
		condition_init(&table->scheduler_condition);
	}
	else
		free(table);
}

static t_table	*create_table(char **args)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (table)
	{
		table->number_of_coders = atoi(args[1]);
		table->dongles = table->number_of_coders;
		if (table->number_of_coders == 1)
			table->dongles = 2;
		table->time_to_burnout = atoi(args[2]);
		table->time_to_compile = atoi(args[3]);
		table->time_to_debug = atoi(args[4]);
		table->time_to_refactor = atoi(args[5]);
		table->compiles_required = atoi(args[6]);
		table->dongle_cooldown = atoi(args[7]);
		pthread_mutex_init(&table->failed_mutex, NULL);
		table->scheduler_finish = 0;
		pthread_mutex_init(&table->scheduler_mutex, NULL);
		if (!strcmp(args[8], EDF_STR))
			table->scheduler = EDF;
		else
			table->scheduler = FIFO;
		table->failed = 0;
		create_queue(table);
		if (!table->queue)
			return (NULL);
	}
	return (table);
}

/*
static void	display_table(t_table *table)
{
	printf("Table created:\n");
	printf("1. number_of_coders: %u\n", table->number_of_coders);
	printf("2. time_to_burnout: %llu\n", table->time_to_burnout);
	printf("3. time_to_compile: %llu\n", table->time_to_compile);
	printf("4. time_to_debug: %llu\n", table->time_to_debug);
	printf("5. time_to_refactor: %llu\n", table->time_to_refactor);
	printf("6. number_of_compiles_required: %u\n", table->compiles_required);
	printf("7. dongle_cooldown: %llu\n", table->dongle_cooldown);
	printf("8. scheduler: %u\n\n", table->scheduler);
}
*/
static t_coder	**create_coders(t_table *table)
{
	t_coder	**coders;
	t_byte	i;

	coders = malloc(sizeof(t_coder *) * table->number_of_coders);
	if (coders)
	{
		i = -1;
		while (++i < table->number_of_coders)
		{
			coders[i] = malloc(sizeof(t_coder));
			coders[i]->id = i;
			coders[i]->action_time = 0;
			coders[i]->deadline = -1;
			coders[i]->thread = 0;
			coders[i]->finished = 0;
			coders[i]->delayed = 0;
			condition_init(&coders[i]->condition);
		}
	}
	return (coders);
}

t_table	*setup_codexion(char **args)
{
	t_table	*table;

	table = NULL;
	if (validate_args(args))
	{
		table = create_table(args);
		if (!table)
			return (NULL);
		table->coders = create_coders(table);
		if (!table->coders)
		{
			free(table);
			return (NULL);
		}
	}
	return (table);
}
