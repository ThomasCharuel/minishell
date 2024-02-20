/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:38:57 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 14:39:29 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command_status	get_var_value(t_state *state, const char **ptr,
		char **word)
{
	const char	*value;
	const char	*cursor;
	char		*var_str;

	cursor = *ptr;
	if (*cursor == '?')
	{
		*word = ft_itoa((int)state->last_exit_code);
		if (!*word)
			return (COMMAND_ERROR);
		*ptr = ++cursor;
		return (COMMAND_SUCCESS);
	}
	if (!ft_isalpha(*cursor) && *cursor != '_')
	{
		*word = ft_calloc(1, 1);
		if (!*word)
			return (COMMAND_ERROR);
		*ptr = cursor;
		return (COMMAND_SUCCESS);
	}
	while (ft_isalnum(*cursor) || *cursor == '_')
		cursor++;
	var_str = ft_strndup(*ptr, cursor - *ptr);
	if (!var_str)
		return (COMMAND_ERROR);
	*ptr = cursor;
	value = envp_get(state, var_str);
	free(var_str);
	*word = ft_strdup(value);
	if (!*word)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	handle_env_var(t_state *state, const char **ptr,
		t_list **words)
{
	t_command_status	status;
	char				*word;

	while (*ptr && **ptr)
	{
		if (**ptr == '$')
		{
			(*ptr)++;
			status = get_var_value(state, ptr, &word);
		}
		else if (**ptr == '\'')
			status = get_next_word(ptr, &word, "\'", true);
		else
			status = get_next_word(ptr, &word, "\'$", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (free(word), COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}
