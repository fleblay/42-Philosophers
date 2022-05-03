/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_satoi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 14:26:50 by fle-blay          #+#    #+#             */
/*   Updated: 2022/05/03 16:04:45 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_isspace(int c)
{
	if (c == '\f' || c == '\t' || c == '\n' || c == '\r' || c == '\v' \
		|| c == ' ')
		return (1);
	return (0);
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

long	ft_satoi(const char *nptr, int *error)
{
	int		i;
	long	nbr;

	i = 0;
	nbr = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '+')
		i++;
	while (ft_isdigit(nptr[i]))
	{
		nbr = 10 * nbr + nptr[i] - 48;
		if (nbr > 2147483647)
		{
			*error = 1;
			break ;
		}
		i++;
	}
	*error += (nptr[i] != '\0');
	return (nbr);
}
