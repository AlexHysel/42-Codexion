/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afomin  afomin@student.42kl.edu.my         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:35:59 by afomin            #+#    #+#             */
/*   Updated: 2026/03/10 14:50:36 by afomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"
#define UINT_MAX_STR "4294967295"
#define UINT_MAX_LEN 10
#define MSEC_MAX_STR "18446744073709551615"
#define MSEC_MAX_LEN 20

static int	is_uint(char *num)
{
	size_t	numlen;
	size_t	i;

	numlen = strlen(num);
	i = 0;
	while (i < numlen)
		if (num[i] < '0' || num[i++] > '9')
			return (0);
	if (numlen < UINT_MAX_LEN)
		return (1);
	if (numlen == UINT_MAX_LEN)
		return (strcmp(num, UINT_MAX_STR) <= 0);
	else
		return (0);
}

static int	is_msec(char *num)
{
	size_t	numlen;
	size_t	i;

	numlen = strlen(num);
	i = 0;
	while (i < numlen)
		if (num[i] < '0' || num[i++] > '9')
			return (0);
	if (numlen < MSEC_MAX_LEN)
		return (1);
	if (numlen == MSEC_MAX_LEN)
		return (strcmp(num, MSEC_MAX_STR) <= 0);
	else
		return (0);
}

/*
number_of_coders			- uint
time_to_burnout				- msec
time_to_compile				- msec
time_to_debug				- msec
time_to_refactor			- msec
number_of_compiles_required	- uint
dongle_cooldown				- msec
scheduler					- string
*/
int	validate_args(char **args)
{
	if (!is_uint(args[1]))
		return (!printf("Number of coders should be unsigned int"));
	if (!is_msec(args[2]))
		return (!printf("Time to burnout should be unsigned long long"));
	if (!is_msec(args[3]))
		return (!printf("Time to compile should be unsigned long long"));
	if (!is_msec(args[4]))
		return (!printf("Time to debug should be unsigned long long"));
	if (!is_msec(args[5]))
		return (!printf("Time to refactor should be unsigned long long"));
	if (!is_uint(args[6]))
		return (!printf("Compiles required should be unsigned int"));
	if (!is_msec(args[7]))
		return (!printf("Dongle cooldown should be unsigned long long"));
	if (strcmp(args[8], FIFO_STR) != 0 && strcmp(args[8], EDF_STR) != 0)
		return (!printf("Scheduler is not %s/%s", EDF_STR, FIFO_STR));
	return (1);
}
