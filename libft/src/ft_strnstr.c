/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:44:33 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/14 15:49:21 by tcharuel         ###   ########.fr       */
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

const char	*ft_strstrn(const char *str, const char *search_str, size_t len)
{
	size_t	str_len;
	size_t	i;

	if (len > ft_strlen(search_str))
		return (NULL);
	str_len = ft_strlen(str);
	i = 0;
	while (str[i] && str_len - i >= len)
	{
		if (!ft_strncmp(&str[i], search_str, len))
			return (&str[i]);
		i++;
	}
	return (NULL);
}
