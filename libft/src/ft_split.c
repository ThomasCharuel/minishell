/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:30 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/13 14:25:37 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_strs_count(char const *s, char c)
{
	size_t	count;
	int		is_new_str;

	is_new_str = 1;
	count = 0;
	while (*s != '\0')
	{
		if (*s == c)
		{
			if (is_new_str == 0)
				is_new_str = 1;
		}
		else if (is_new_str == 1)
		{
			is_new_str = 0;
			count++;
		}
		s++;
	}
	return (count);
}

static size_t	strlen_with_delimiter(const char *s, char c)
{
	size_t	size;

	size = 0;
	while (s[size] != '\0' && s[size] != c)
		size++;
	return (size);
}

static char	*strdup_and_move_with_delimiter(char const **s, char c)
{
	char	*dup;
	size_t	s_len;
	size_t	i;

	while (**s == c)
		(*s)++;
	s_len = strlen_with_delimiter(*s, c);
	dup = (char *)malloc((s_len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < s_len)
	{
		dup[i++] = **s;
		(*s)++;
	}
	dup[i] = '\0';
	return (dup);
}

/**
 * @brief Allocates (with malloc(3)) and returns an array
 * of strings obtained by splitting ’s’ using the
 * character ’c’ as a delimiter. The array must end
 * with a NULL pointer.
 *
 * @param s The string to be split.
 * @param c The delimiter character.
 *
 * @return The array of new strings resulting from the split.
 * @retval NULL if the allocation fails.
 */
char	**ft_split(char const *s, char c)
{
	char	**strs;
	size_t	strs_count;
	int		i;

	if (!s)
		return (NULL);
	strs_count = get_strs_count(s, c);
	strs = (char **)malloc((strs_count + 1) * sizeof(char *));
	if (!strs)
		return (NULL);
	i = 0;
	while ((size_t)i < strs_count)
	{
		strs[i] = strdup_and_move_with_delimiter(&s, c);
		if (!strs[i])
		{
			while (--i >= 0)
				free(strs[i]);
			free(strs);
			return (NULL);
		}
		i++;
	}
	strs[strs_count] = NULL;
	return (strs);
}
