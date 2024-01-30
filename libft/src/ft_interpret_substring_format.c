/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interpret_substring_format.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 20:11:49 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/30 12:44:37 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_string_format(va_list args)
{
	char	*str_arg;
	char	*result;

	str_arg = va_arg(args, char *);
	if (str_arg)
		result = ft_strdup(str_arg);
	else
		result = ft_strdup("(null)");
	return (result);
}

char	*get_char_format(va_list args)
{
	char	*result;

	result = (char *)malloc(2 * sizeof(char));
	if (!result)
		return (NULL);
	result[0] = va_arg(args, int);
	result[1] = '\0';
	return (result);
}

char	*get_percent_format(void)
{
	char	*result;

	result = (char *)malloc(2 * sizeof(char));
	if (!result)
		return (NULL);
	result[0] = '%';
	result[1] = '\0';
	return (result);
}

char	*get_pointer_format(va_list args)
{
	uintptr_t	ptr;
	char		*hex_string;
	char		*result;

	ptr = va_arg(args, uintptr_t);
	if (ptr)
	{
		hex_string = ft_lutoa(ptr, BASE_HEX_LOWERCASE);
		result = (char *)malloc((ft_strlen(hex_string) + 3) * sizeof(char));
		if (!result)
			return (NULL);
		result[0] = '0';
		result[1] = 'x';
		result[2] = '\0';
		ft_strcat(result, hex_string);
		free(hex_string);
	}
	else
		result = ft_strdup("(nil)");
	return (result);
}
