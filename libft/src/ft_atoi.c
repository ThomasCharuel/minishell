/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:09:21 by tcharuel          #+#    #+#             */
/*   Updated: 2023/12/02 17:08:26 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isspace(const char c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		sign;
	int		res;

	sign = 1;
	res = 0;
	i = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign = -1;
	while (ft_isdigit(nptr[i]))
	{
		res *= 10;
		res += (nptr[i] - 48);
		i++;
	}
	return (sign * res);
}

long	ft_atol(const char *nptr)
{
	size_t	i;
	int		sign;
	long	res;

	sign = 1;
	res = 0;
	i = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign = -1;
	while (ft_isdigit(nptr[i]))
	{
		res *= 10;
		res += (nptr[i] - 48);
		i++;
	}
	return (sign * res);
}
