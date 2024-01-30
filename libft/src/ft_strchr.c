/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:23:26 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/07 18:40:04 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	byte;

	byte = (unsigned char)c;
	while (*s != '\0' && *s != byte)
		s++;
	if (*s == byte)
		return ((char *)s);
	return (NULL);
}
