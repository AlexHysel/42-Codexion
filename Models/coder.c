/* ************************************************************************** */
/*																		      */
/*														:::	  ::::::::        */
/*   coder.c											:+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: afomin  afomin@student.42kl.edu.my		 +#+  +:+	   +#+		      */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2026/03/15 14:57:45 by afomin			#+#	#+#			          */
/*   Updated: 2026/03/15 14:57:46 by afomin		   ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "../codexion.h"

static void	update_time(t_coder *coder, t_msec time_to_burnout)
{
	coder->last_compile = current_time_ms();
	pthread_mutex_lock(&coder->mutex);
	coder->deadline = coder->last_compile + time_to_burnout;
	pthread_mutex_unlock(&coder->mutex);
}

t_byte	is_dead(t_coder *coder)
{
	t_byte	is_dead;

	pthread_mutex_lock(&coder->mutex);
	is_dead = !coder->finished && coder->deadline <= current_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	return (is_dead);
}

static void	compile_state(t_coder *coder, t_table *table)
{
	t_msec	time;

	take_dongles(coder, table);
	if (!is_failed(table))
	{
		update_time(coder, table->time_to_burnout);
		add_log(table->logger, "is compiling", coder->id);
		delay(table->time_to_compile);
		time = current_time_ms();
		pthread_mutex_lock(&coder->right_dongle->mutex);
		coder->right_dongle->is_taken = 0;
		coder->right_dongle->cooldown_end = time + table->dongle_cooldown;
		pthread_mutex_lock(&coder->left_dongle->mutex);
		coder->left_dongle->is_taken = 0;
		coder->left_dongle->cooldown_end = time + table->dongle_cooldown;
	}
	broadcast(coder->right_dongle->cond, &coder->right_dongle->mutex);
	broadcast(coder->left_dongle->cond, &coder->left_dongle->mutex);
}

static void	life_loop(t_table *table, t_coder *coder)
{
	t_byte	compiles_done;

	compiles_done = 0;
	while (!is_failed(table))
	{
		compile_state(coder, table);
		if (++compiles_done == table->compiles_required || is_failed(table))
			break ;
		add_log(table->logger, "is debugging", coder->id);
		delay(table->time_to_debug);
		if (is_failed(table))
			break ;
		add_log(table->logger, "is refactoring", coder->id);
		delay(table->time_to_refactor);
	}
}

void	*c_life(void *thread_data)
{
	t_table	*table;
	t_coder	*coder;

	table = ((t_thread_data *) thread_data)->table;
	coder = ((t_thread_data *) thread_data)->coder;
	free(thread_data);
	update_time(coder, table->time_to_burnout);
	if (coder->id % 2)
		delay(table->time_to_compile / 2);
	life_loop(table, coder);
	pthread_mutex_lock(&coder->mutex);
	coder->finished = 1;
	pthread_mutex_unlock(&coder->mutex);
	return (NULL);
}
