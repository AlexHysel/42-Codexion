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

static void	print_not_enough_args(void)
{
	printf("Not enough arguments provided.\n");
	printf("Arguments:\n");
	printf("1. number_of_coders\n2. time_to_burnout\n");
	printf("3. time_to_compile\n4. time_to_debug\n");
	printf("5. time_to_refactor\n6. number_of_compiles_required\n");
	printf("7. dongle_cooldown\n8. scheduler");
}

static t_table	*create_table(char **args)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (table)
	{
		table->number_of_coders = atoi(args[1]);
		table->dongles = table->number_of_coders;
		table->time_to_burnout = atoi(args[2]);
		table->time_to_compile = atoi(args[3]);
		table->time_to_debug = atoi(args[4]);
		table->time_to_refactor = atoi(args[5]);
		table->compiles_required = atoi(args[6]);
		table->dongle_cooldown = atoi(args[7]);
		if (!strcmp(args[8], EDF_STR))
			table->scheduler = EDF;
		else
			table->scheduler = FIFO;
		table->failed = 0;
		table->queue = malloc(sizeof(t_requestQueue));
		pthread_mutex_init(&table->dongle_mutex, NULL);
		pthread_mutex_init(&table->queue->mutex, NULL);
		pthread_cond_init(&table->condition, NULL);
		pthread_cond_init(&table->scheduler_condition, NULL);
	}
	return (table);
}

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
			pthread_cond_init(&coders[i]->condition, NULL);
		}
	}
	return (coders);
}

int	app_run(int argc, char **args)
{
	t_table	*table;

	if (argc != 9)
		print_not_enough_args();
	else if (validate_args(args))
	{
		table = create_table(args);
		if (!table)
			return (error("During table creating."));
		display_table(table);
		table->coders = create_coders(table);
		if (!table->coders)
			return (error("During coders creating."));
		run_codexion(table);
		pthread_mutex_destroy(&table->dongle_mutex);
		pthread_mutex_destroy(&table->queue->mutex);
		pthread_cond_destroy(&table->condition);
		pthread_cond_destroy(&table->scheduler_condition);
	}
	return (1);
}
