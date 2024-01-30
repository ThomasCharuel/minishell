/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:22:09 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/06 18:16:19 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	byte;
	unsigned char	*src;
	size_t			i;

	byte = (unsigned char)c;
	src = (unsigned char *)s;
	i = 0;
	while (i < n)
		src[i++] = byte;
	return (s);
}
