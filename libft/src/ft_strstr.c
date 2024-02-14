/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:29:14 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/14 13:39:21 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const char	*ft_strstr(const char *str, const char *search_string)
{
	while (*str)
		if (!ft_strcmp(str++, search_string))
			return (str);
	return (NULL);
}

bool	ft_str_endswith(const char *str, const char *search_str)
{
	size_t	str_len;
	size_t	search_str_len;

	str_len = ft_strlen(str);
	search_str_len = ft_strlen(search_str);
	if (search_str_len > str_len)
		return (false);
	return (!ft_strcmp(search_str, str + str_len - search_str_len));
}
