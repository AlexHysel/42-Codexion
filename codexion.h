/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin <afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#define EDF_STR "edf"
#define FIFO_STR "fifo"

typedef unsigned int		t_uint;
typedef unsigned long long	t_msec;

typedef enum e_state
{
	COMPILING = 1,
	DEBUGGING = 2,
	REFACTORING = 3
}	t_state;

typedef struct s_coder
{
	t_uint	id;
	t_state	state;
	int		is_dead;
	t_uint	compilations_done;
	t_msec	last_action_time;
}	t_coder;

typedef struct s_table
{
	t_uint	number_of_coders;
	t_msec	time_to_burnout;
	t_msec	time_to_compile;
	t_msec	time_to_debug;
	t_msec	time_to_refactor;
	t_uint	compiles_required;
	t_msec	dongle_cooldown;
}	t_table;

// ===== ConsoleUI =====
int		validate_args(char **args);
void	console_ui_run(int argc, char **args);