/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin  afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	parse_args(t_table *table, char **args)
{
	table->number_of_coders = atoi(args[1]);
	table->time_to_burnout = atoi(args[2]);
	table->time_to_compile = atoi(args[3]);
	table->time_to_debug = atoi(args[4]);
	table->time_to_refactor = atoi(args[5]);
	table->compiles_required = atoi(args[6]);
	table->dongle_cooldown = atoi(args[7]);
	if (!strcmp(args[8], EDF_STR))
		table->type = EDF;
	if (!strcmp(args[8], FIFO_STR))
		table->type = FIFO;
}

static t_table	*create_table(char **args)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (table)
	{
		parse_args(table, args);
		table->failed = 0;
		pthread_mutex_init(&table->mutex, NULL);
		condition_init(&table->condition);
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
	printf("8. scheduler: %u\n\n", table->scheduler->type);
}
*/

static t_coder	**create_coders(t_table *table)
{
	t_coder		**coders;
	t_byte		i;

	coders = malloc(sizeof(t_coder *) * table->number_of_coders);
	if (coders)
	{
		i = -1;
		while (++i < table->number_of_coders)
		{
			coders[i] = malloc(sizeof(t_coder));
			coders[i]->id = i;
			coders[i]->last_compile = 0;
			coders[i]->deadline = -1;
			coders[i]->thread = 0;
			coders[i]->finished = 0;
			dongle_init(&coders[i]->left_dongle);
			if (i)
				coders[i - 1]->right_dongle = coders[i]->left_dongle;
			pthread_mutex_init(&coders[i]->mutex, NULL);
		}
		coders[i - 1]->right_dongle = coders[0]->left_dongle;
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
		if (table)
		{
			table->coders = create_coders(table);
			if (!table->coders)
				cleanup(table);
		}
	}
	return (table);
}
