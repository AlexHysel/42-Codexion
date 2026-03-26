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

static int	python_sucks(char **args)
{
	int			status;
	t_logger	*logger;
	t_table		*table;

	status = 0;
	logger = run_logger();
	if (logger)
	{
		table = setup_codexion(args);
		if (table && table->scheduler && table->scheduler->queue)
		{
			table->logger = logger;
			run_codexion(table);
			cleanup(table);
		}
		else
			status = 1;
		stop_logger(logger);
	}
	else
		status = 1;
	return (status);
}

int	main(int argc, char **args)
{
	if (argc != 9)
	{
		write(2, "Error\n", 6);
		write(2, "Usage: ./codexion number_of_coders time_to_burnout ", 51);
		write(2, "time_to_compile time_to_debug time_to_refactor ", 47);
		write(2, "num_compiles_required [dongle_cooldown] [scheduler]\n", 52);
		return (1);
	}
	else if (validate_args(args))
		return (python_sucks(args));
	return (1);
}
