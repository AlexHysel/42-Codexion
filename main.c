/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin <afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	print_not_enough_args(void)
{
	printf("Not enough arguments provided.\n");
	printf("Arguments:\n");
	printf("1. number_of_coders\n2. time_to_burnout\n");
	printf("3. time_to_compile\n4. time_to_debug\n");
	printf("5. time_to_refactor\n6. number_of_compiles_required\n");
	printf("7. dongle_cooldown\n8. scheduler");
}

int	main(int argc, char **args)
{
	t_table		*table;
	t_logger	*logger;

	logger = NULL;
	if (argc != 9)
		print_not_enough_args();
	else if (validate_args(args))
	{
		logger = run_logger();
		if (logger)
		{
			table = setup_codexion(args);
			if (table)
			{
				table->logger = logger;
				run_codexion(table);
				cleanup(table);
			}
		}
		stop_logger(logger);
	}
}
