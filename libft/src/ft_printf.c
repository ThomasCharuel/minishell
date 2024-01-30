/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:01:08 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/30 12:44:47 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Each conversion specification is introduced by the character %
 * and ends with a conversion specifier.
 *
 * %c Prints a single character.
 * %s Prints a string (as defined by the common C convention).
 * %p The void * pointer argument has to be printed in hexadecimal format.
 * %d Prints a decimal (base 10) number.
 * %i Prints an integer in base 10.
 * %u Prints an unsigned decimal (base 10) number.
 * %x Prints a number in hexadecimal (base 16) lowercase format.
 * %X Prints a number in hexadecimal (base 16) uppercase format.
 * %% Prints a percent sign
 *
 * @param lst: The address of a pointer to the first link of
 * a list.
 * @param new: The address of a pointer to the node to be
 * added to the list.
 *
 * @return The number of bytes written to STDOUT
 * @retval -1 in case of error.
 **/
int	ft_printf(const char *format, ...)
{
	t_list	*substring_list;
	size_t	len;
	va_list	args;

	if (!format)
		return (-1);
	va_start(args, format);
	substring_list = parse_format(format, args);
	if (!substring_list)
		return (-1);
	va_end(args);
	ft_lstiter(substring_list, (void *)put_substring_result);
	len = get_len_substrings(substring_list);
	ft_lstclear(&substring_list, (void *)free_substring);
	return (len);
}
