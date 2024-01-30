/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lutoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:05:20 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/17 19:13:47 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_nb_digits(unsigned long nb, int radix)
{
	if (nb / radix == 0)
		return (1);
	return (1 + get_nb_digits(nb / radix, radix));
}

static void	put_n_in_str(unsigned long nb, char *res, size_t len, char *base)
{
	int	radix;

	radix = ft_strlen(base);
	res[len--] = '\0';
	if (nb == 0)
		res[len] = base[0];
	while (nb > 0)
	{
		res[len--] = base[nb % radix];
		nb /= radix;
	}
}

/**
 * @brief Allocates (with malloc(3)) and returns a string
 * representing the unsigned long received as an argument..
 *
 * @param n The unsigned long to convert.
 *
 * @return The string representing the unsigned long.
 * @retval NULL if the allocation fails.
 */
char	*ft_lutoa(unsigned long n, char *base)
{
	int		radix;
	char	*res;
	size_t	len;

	radix = ft_strlen(base);
	len = get_nb_digits(n, radix);
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	put_n_in_str(n, res, len, base);
	return (res);
}
