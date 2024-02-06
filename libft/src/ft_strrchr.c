/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:23:26 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/06 19:18:12 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	byte;
	char			*found_char;

	found_char = NULL;
	byte = (unsigned char)c;
	while (*s != '\0')
	{
		if (*s == byte)
			found_char = (char *)s;
		s++;
	}
	if (*s == byte)
		found_char = (char *)s;
	return (found_char);
}

bool	ft_is_char_in_set(const char c, const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (c == s[i++])
			return (true);
	}
	return (false);
}

const char	*ft_strchrs(const char *s, const char *set)
{
	while (*s)
	{
		if (ft_is_char_in_set(*s, set))
			return (s);
		s++;
	}
	return (NULL);
}
