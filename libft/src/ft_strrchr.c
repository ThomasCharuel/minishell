/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:23:26 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/07 10:53:37 by tcharuel         ###   ########.fr       */
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
