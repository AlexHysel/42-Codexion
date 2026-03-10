/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_ui.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin <afomin@student.42kl.edu.my         +#+  +:+       +#+        */
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
	printf("number_of_coders\ntime_to_burnout\n");
	printf("time_to_compile\ntime_to_debug\n");
	printf("time_to_refactor\nnumber_of_compiles_required\n");
	printf("dongle_cooldown\nscheduler");
}

void	console_ui_run(int argc, char **args)
{
	if (argc == 9)
	{
		validate_args(args);
	}
	else
		print_not_enough_args();
}
