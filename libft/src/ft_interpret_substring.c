/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interpret_substring.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 19:33:21 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/30 12:44:40 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_conversion_result(char conversion, va_list args)
{
	char	*result;

	if (conversion == CONVERSION_CHAR_STRING)
		result = get_string_format(args);
	else if (conversion == CONVERSION_CHAR_CHARACTER)
		result = get_char_format(args);
	else if (conversion == CONVERSION_CHAR_PERCENT)
		result = get_percent_format();
	else if (conversion == CONVERSION_CHAR_POINTER)
		result = get_pointer_format(args);
	else if (conversion == CONVERSION_CHAR_HEX_UPPERCASE)
		result = ft_ltoa((unsigned int)va_arg(args, int), BASE_HEX_UPPERCASE);
	else if (conversion == CONVERSION_CHAR_HEX_LOWERCASE)
		result = ft_ltoa((unsigned int)va_arg(args, int), BASE_HEX_LOWERCASE);
	else if (conversion == CONVERSION_CHAR_DECIMAL
		|| conversion == CONVERSION_CHAR_INTEGER)
		result = ft_itoa(va_arg(args, int));
	else if (conversion == CONVERSION_CHAR_UNSIGNED_DECIMAL)
		result = ft_ltoa(va_arg(args, unsigned int), BASE_DECIMAL);
	else
		result = ft_strdup("");
	return (result);
}

int	interpret_substring(t_substring *substring, va_list args)
{
	size_t	i;
	char	conversion;

	conversion = 0;
	if (substring->format[0] == '%')
	{
		i = 0;
		while (substring->format[i + 1])
			i++;
		conversion = substring->format[i];
		substring->result = get_conversion_result(conversion, args);
	}
	else
		substring->result = ft_strdup(substring->format);
	if (!substring->result)
		return (-1);
	if (conversion == CONVERSION_CHAR_CHARACTER && substring->result[0] == '\0')
		substring->result_length = 1;
	else
		substring->result_length = ft_strlen(substring->result);
	return (1);
}
