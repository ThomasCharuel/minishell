/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:25:05 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 17:06:29 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*buffer_dest;
	int				i;

	if (dest == src || n == 0)
		return (dest);
	buffer_dest = (unsigned char *)dest;
	if (dest < src)
	{
		i = 0;
		while ((size_t)i < n)
		{
			buffer_dest[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (--i >= 0)
			buffer_dest[i] = ((unsigned char *)src)[i];
	}
	return (dest);
}
