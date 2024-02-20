/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:34:29 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 15:49:37 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static t_command_status	trim_quotes(char **str)
{
	char	*trimmed_str;

	if (!ft_is_char_in_set(**str, "\'\""))
		return (COMMAND_SUCCESS);
	trimmed_str = ft_strndup(&(*str)[1], ft_strlen(&(*str)[1]) - 1);
	if (!trimmed_str)
		return (COMMAND_ERROR);
	free(*str);
	*str = trimmed_str;
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	remove_quotes(t_state *state, const char **ptr,
		t_list **words)
{
	t_command_status	status;
	char				*word;

	(void)state;
	while (*ptr && **ptr)
	{
		if (ft_is_char_in_set(**ptr, "\'\""))
			status = get_next_word_char(ptr, &word, **ptr, true);
		else
			status = get_next_word(ptr, &word, "\'\"", false);
		if (status)
			return (status);
		status = trim_quotes(&word);
		if (status)
			return (free(word), status);
		if (!str_list_append(words, word))
			return (free(word), COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}
