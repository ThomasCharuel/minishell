/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:49:29 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 17:34:09 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_charset(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i] != '\0')
		if (c == set[i++])
			return (1);
	return (0);
}

static size_t	strlen_with_charset(char const *s, char const *set)
{
	size_t	i;

	i = ft_strlen(s);
	while (i > 0 && is_charset(s[i - 1], set))
		if (i > 0)
			i--;
	return (i);
}

/**
 * @brief Allocates (with malloc(3)) and returns a copy of
 * ’s1’ with the characters specified in ’set’ removed
 * from the beginning and the end of the string.
 *
 * @param s1 The string to be trimmed.
 * @param set The reference set of characters to trim.
 *
 * @return The trimmed string.
 * @retval NULL if the allocation fails.
 */
char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed_s;
	size_t	i;
	size_t	trimmed_s_len;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	while (is_charset(s1[i], set))
		i++;
	trimmed_s_len = strlen_with_charset(&s1[i], set);
	trimmed_s = (char *)malloc((trimmed_s_len + 1) * sizeof(char));
	if (!trimmed_s)
		return (NULL);
	ft_strlcpy(trimmed_s, &s1[i], trimmed_s_len + 1);
	return (trimmed_s);
}
