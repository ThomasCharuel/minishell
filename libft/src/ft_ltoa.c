/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:05:20 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 10:40:25 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_nb_digits(long long nb, int radix)
{
	if (nb / radix == 0)
		return (1);
	return (1 + get_nb_digits(nb / radix, radix));
}

static size_t	get_result_length(long long nb, int radix)
{
	size_t	len;

	if (nb < 0)
	{
		len = 1;
		nb *= -1;
	}
	else
		len = 0;
	len += get_nb_digits(nb, radix);
	return (len);
}

static void	put_n_in_str(long long nb, char *res, size_t len, char *base)
{
	int	radix;

	radix = ft_strlen(base);
	if (nb < 0)
	{
		res[0] = '-';
		nb *= -1;
	}
	if (nb == 0)
		res[0] = base[0];
	while (nb > 0)
	{
		res[--len] = base[nb % radix];
		nb /= radix;
	}
}

/**
 * @brief Allocates (with malloc(3)) and returns a string
 * representing the long received as an argument.
 * Negative numbers must be handled.
 *
 * @param n The long to convert.
 *
 * @return The string representing the long.
 * @retval NULL if the allocation fails.
 */
char	*ft_ltoa(long n, char *base)
{
	int		radix;
	char	*res;
	size_t	len;

	radix = ft_strlen(base);
	len = get_result_length(n, radix);
	res = ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (NULL);
	put_n_in_str(n, res, len, base);
	return (res);
}

/**
 * @brief Allocates (with malloc(3)) and returns a string
 * representing the integer received as an argument.
 * Negative numbers must be handled.
 *
 * @param n The integer to convert.
 *
 * @return The string representing the integer.
 * @retval NULL if the allocation fails.
 */
char	*ft_itoa(int n)
{
	return (ft_ltoa(n, BASE_DECIMAL));
}
