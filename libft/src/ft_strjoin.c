/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:50:16 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/04 18:22:20 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocates (with malloc(3)) and returns a new
 * string, which is the result of the concatenation
 * of ’s1’ and ’s2’.
 *
 * @param s1  The prefix string.
 * @param s2 The suffix string.
 *
 * @return The newly allocated string.
 * @retval NULL if the allocation fails.
 */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*buffer;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	buffer = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	ft_strlcpy(buffer, s1, s1_len + 1);
	ft_strlcat(buffer, s2, s1_len + s2_len + 1);
	return (buffer);
}

static size_t	calculate_total_length(char *str, va_list args)
{
	size_t	length;

	length = 0;
	while (str)
	{
		length += ft_strlen(str);
		str = va_arg(args, char *);
	}
	return (length);
}

char	*ft_strsjoin(char *str, ...)
{
	va_list	args;
	char	*res;
	size_t	len;

	if (!str)
		return (NULL);
	va_start(args, str);
	len = calculate_total_length(str, args);
	va_end(args);
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[0] = 0;
	va_start(args, str);
	while (str)
	{
		ft_strcat(res, str);
		str = va_arg(args, char *);
	}
	va_end(args);
	return (res);
}
