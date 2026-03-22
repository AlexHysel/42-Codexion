/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin afomin@student.42kl.edu.my          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 18:36:37 by afomin            #+#    #+#             */
/*   Updated: 2026/03/12 18:36:38 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

t_msec	current_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return ((t_msec)tv.tv_sec * 1000 + (t_msec)tv.tv_usec / 1000);
}

int	error(char *msg)
{
	size_t	len;
	char	*error;
	char	nl;

	len = strlen(msg);
	error = "ERROR: ";
	nl = '\n';
	write(2, error, 7);
	write(2, msg, len);
	write(2, &nl, 1);
	return (0);
}

void	logger(char *msg, t_byte id, t_msec start_time)
{
	static t_mutex	log_mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&log_mutex);
	printf("%llu ", current_time_ms() - start_time);
	if (id != 255)
		printf("%d ", id);
	printf("%s\n", msg);
	pthread_mutex_unlock(&log_mutex);
}

void	delay(t_msec milliseconds)
{
	usleep(milliseconds * 1000);
}
