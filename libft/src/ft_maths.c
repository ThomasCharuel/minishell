/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_maths.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 22:34:34 by tcharuel          #+#    #+#             */
/*   Updated: 2023/12/03 22:52:24 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_min(int count, ...)
{
	va_list	args;
	int		min_value;
	int		value;
	int		i;

	va_start(args, count);
	min_value = INT_MAX;
	i = 0;
	while (i < count)
	{
		value = va_arg(args, int);
		if (value < min_value)
			min_value = value;
		i++;
	}
	va_end(args);
	return (min_value);
}

int	ft_max(int count, ...)
{
	va_list	args;
	int		max_value;
	int		value;
	int		i;

	va_start(args, count);
	max_value = INT_MIN;
	i = 0;
	while (i < count)
	{
		value = va_arg(args, int);
		if (value > max_value)
			max_value = value;
		i++;
	}
	va_end(args);
	return (max_value);
}
