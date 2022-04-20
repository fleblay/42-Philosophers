/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:17:42 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/20 16:13:58 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <pthread.h>

int	print(void)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		printf("%d\n", i);
		i++;
	}
	return (i);
}

int	main(void)
{
	pthread th1;
	printf("Hello World\n");
	pthread_create(&th1, NULL, print, NULL);
	return (1);
}
