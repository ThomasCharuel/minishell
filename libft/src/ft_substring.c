/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substring.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 11:04:51 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/30 12:46:02 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_substring	*create_substring(char *format, size_t format_length)
{
	t_substring	*substring;

	substring = (t_substring *)malloc(sizeof(t_substring));
	if (!substring)
		return (NULL);
	substring->format = format;
	substring->format_length = format_length;
	substring->result = NULL;
	substring->result_length = 0;
	return (substring);
}

void	free_substring(t_substring *substring)
{
	substring = (t_substring *)substring;
	if ((substring)->format)
		free((substring)->format);
	if ((substring)->result)
		free((substring)->result);
	if (substring)
		free(substring);
}

void	put_substring_result(t_substring *substring)
{
	if (substring->result_length > 0)
		write(STDOUT_FILENO, substring->result, substring->result_length);
}

int	get_len_substrings(t_list *lst)
{
	int	length;
	int	node_length;

	length = 0;
	while (lst)
	{
		node_length = ((t_substring *)lst->content)->result_length;
		if (node_length >= 0)
		{
			if (length >= 0)
				length += ((t_substring *)lst->content)->result_length;
		}
		else
			length = node_length;
		lst = lst->next;
	}
	return (length);
}
