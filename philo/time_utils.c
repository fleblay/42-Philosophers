/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:25:00 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/02 15:57:14 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stddef.h>

unsigned long	ft_get_time(void)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}
