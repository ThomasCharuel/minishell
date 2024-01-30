/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_format.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 19:21:06 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/30 12:44:43 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_conversion_identifier(const char c)
{
	if (c == CONVERSION_CHAR_CHARACTER || c == CONVERSION_CHAR_STRING
		|| c == CONVERSION_CHAR_POINTER || c == CONVERSION_CHAR_DECIMAL
		|| c == CONVERSION_CHAR_INTEGER || c == CONVERSION_CHAR_UNSIGNED_DECIMAL
		|| c == CONVERSION_CHAR_HEX_LOWERCASE
		|| c == CONVERSION_CHAR_HEX_UPPERCASE || c == CONVERSION_CHAR_PERCENT)
		return (1);
	return (0);
}

size_t	get_substring_length(const char *format)
{
	size_t	len;

	len = 0;
	if (format[len++] == '%')
	{
		if (is_conversion_identifier(format[len]))
			len++;
	}
	else
		while (format[len] && format[len] != '%')
			len++;
	return (len);
}

t_substring	*parse_substring(const char **format, va_list args)
{
	t_substring	*substring;
	size_t		len;
	char		*substring_format;

	len = get_substring_length(*format);
	substring_format = ft_strndup(*format, len);
	if (!substring_format)
		return (NULL);
	substring = create_substring(substring_format, len);
	if (!substring)
		return (NULL);
	*format += len;
	if (interpret_substring(substring, args) < 0)
	{
		free_substring(substring);
		return (NULL);
	}
	return (substring);
}

t_list	*parse_format(const char *format, va_list args)
{
	t_list		*lst;
	t_list		*new_node;
	t_substring	*substring;

	lst = NULL;
	new_node = NULL;
	while (!new_node || *format)
	{
		substring = parse_substring(&format, args);
		if (substring)
			new_node = ft_lstnew(substring);
		if (!substring || !new_node)
		{
			if (substring)
				free_substring(substring);
			ft_lstclear(&lst, (void *)free_substring);
			return (NULL);
		}
		ft_lstadd_back(&lst, new_node);
	}
	if (new_node && ((t_substring *)new_node->content)->format_length == 1
		&& ((t_substring *)new_node->content)->format[0] == '%')
		((t_substring *)new_node->content)->result_length = -1;
	return (lst);
}
