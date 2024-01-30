/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:55:54 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 15:38:06 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocates and returns a substring from the input string.
 *
 * This function allocates memory for and returns a new substring from the
 * input string 's'. The substring starts at the specified 'start' index
 * and has a maximum length of 'len' characters.
 *
 * @param s The string from which to create the substring.
 * @param start The start index of the substring in the string 's'.
 * @param len The maximum length of the substring.
 *
 * @return The newly allocated substring.
 * @retval NULL if the allocation fails.
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	substr_len;
	size_t	i;

	if (!s || start > ft_strlen(s))
		substr_len = 0;
	else
	{
		s += start;
		substr_len = ft_strlen(s);
		if (len < substr_len)
			substr_len = len;
	}
	substr = (char *)malloc((substr_len + 1) * sizeof(char));
	if (!substr)
		return (NULL);
	i = 0;
	while (i < substr_len)
	{
		substr[i] = s[i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
