/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 00:56:21 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 01:35:05 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static t_command_status	get_next_parenthesis_expression(t_state *state,
		const char **ptr, t_list **words)
{
	t_command_status	status;
	char				*word;
	int					open_parenthesis;

	open_parenthesis = 1;
	(*ptr)++;
	(void)state;
	while (*ptr && **ptr)
	{
		if (**ptr == ')' && --open_parenthesis == 0)
			break ;
		if (**ptr == '(')
			open_parenthesis++;
		if (ft_is_char_in_set(**ptr, "\'\""))
			status = get_next_word_char(ptr, &word, **ptr, true);
		else
			status = get_next_word(ptr, &word, "\'\"()", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (free(word), COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	handle_subshell(t_state *state, t_command *command,
		const char **cursor)
{
	char				*word;
	t_command_status	status;

	word = ft_strdup(state->executable_path);
	if (!word)
		return (COMMAND_ERROR);
	if (!str_list_append(&command->argv, word))
		return (free(word), COMMAND_ERROR);
	status = handle_word(state, cursor, &word,
			&get_next_parenthesis_expression);
	if (status)
		return (status);
	if (!str_list_append(&command->argv, word))
		return (free(word), COMMAND_ERROR);
	(*cursor)++;
	return (COMMAND_SUCCESS);
}
