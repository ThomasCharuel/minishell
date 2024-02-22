/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:38:57 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/22 11:55:41 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static t_command_status	handle_exit_value_var(t_state *state, const char **ptr,
		char **word)
{
	*word = ft_itoa((int)state->last_exit_code);
	if (!*word)
		return (perror("minishell"), COMMAND_ERROR);
	return ((*ptr)++, COMMAND_SUCCESS);
}

// OK
static t_command_status	handle_empty_var(char **word)
{
	*word = ft_calloc(1, 1);
	if (!*word)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	get_var_value(t_state *state, const char **ptr, char **word)
{
	const char	*value;
	char		*var_str;
	size_t		i;

	if (**ptr == '?')
		return (handle_exit_value_var(state, ptr, word));
	if (!ft_isalpha(**ptr) && **ptr != '_')
		return (handle_empty_var(word));
	i = 0;
	while (ft_isalnum((*ptr)[i]) || (*ptr)[i] == '_')
		i++;
	var_str = ft_strndup(*ptr, i);
	if (!var_str)
		return (COMMAND_ERROR);
	*ptr += i;
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
		// C'est faux ca, pourquoi get_next_word de espace
		if (**ptr == '$' && ft_is_char_in_set(*(*ptr + 1),
				"=_+*()[]{}|&\\,.;:!@#^\"\'"))
			status = get_next_word(ptr, &word, " ", false);
		else if (**ptr == '$')
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
			return (ft_free_str(&word), COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}
