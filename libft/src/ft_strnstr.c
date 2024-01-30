/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:44:33 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 16:33:55 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	little_length;

	little_length = ft_strlen(little);
	if (little_length == 0)
		return ((char *)big);
	i = 0;
	if (!len)
		return (NULL);
	while (big[i] != '\0' && i + little_length <= len)
	{
		if (!ft_strncmp(&big[i], little, little_length))
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
