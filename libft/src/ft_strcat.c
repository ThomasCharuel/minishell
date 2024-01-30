/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 19:36:51 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/17 19:12:03 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	size_t	dest_len;
	size_t	i;

	dest_len = ft_strlen(dest);
	i = 0;
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char	*dest_terminator_ptr;
	size_t	dest_len;
	size_t	i;

	if (!size)
		return (ft_strlen(src));
	dest_len = ft_strlen(dst);
	if (size <= dest_len)
		return (size + ft_strlen(src));
	i = 0;
	dest_terminator_ptr = dst + dest_len;
	while (src[i] != '\0')
	{
		if (dest_len + i < size - 1)
		{
			dst[dest_len + i] = src[i];
			dest_terminator_ptr++;
		}
		i++;
	}
	*dest_terminator_ptr = '\0';
	return (dest_len + i);
}
