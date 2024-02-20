/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 20:49:59 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 19:17:47 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
t_command	*command_create(const char *command_str)
{
	t_command	*command;

	command = (t_command *)malloc(sizeof(t_command));
	if (!command)
		return (perror("minishell"), NULL);
	command->command_str = ft_strdup(command_str);
	if (!command->command_str)
		return (perror("minishell"), free(command), NULL);
	command->argv = NULL;
	command->redirections = NULL;
	return (command);
}

// OK
void	command_destroy(void *ptr)
{
	t_command	*command;

	command = ptr;
	if (command)
	{
		free(command->command_str);
		ft_lstclear(&command->argv, free);
		ft_lstclear(&command->redirections, redirection_destroy);
		free(command);
	}
}

t_command_status	handle_command_word(t_state *state, const char **ptr,
		t_command *command)
{
	t_command_status	status;
	char				*word;

	status = handle_word(NULL, ptr, &word, &get_next_token);
	if (status || !word)
		return (status);
	status = handle_word(state, (const char **)&word, &word, &handle_env_var);
	if (status || !word)
		return (status);
	if (command->argv)
	{
		status = handle_wildcards(&word, command->argv);
		if (status || !word)
			return (status);
	}
	status = handle_word(NULL, (const char **)&word, &word, &remove_quotes);
	if (status)
		return (status);
	if (!str_list_append(&command->argv, word))
		return (ft_free_str(&word), COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

t_command_status	command_parse(t_state *state, t_command *command)
{
	t_command_status	status;
	const char			*cursor;

	cursor = command->command_str;
	while (*cursor)
	{
		while (*cursor == ' ')
			cursor++;
		if (*cursor == '(')
		{
			if (command->argv)
				return (print_error("syntax error: unexpected parenthesis",
						NULL), COMMAND_PARSING_ERROR);
			status = handle_subshell(state, command, &cursor);
		}
		else if (ft_is_char_in_set(*cursor, "<>"))
			status = handle_redirection(state, &cursor, command);
		else if (*cursor)
			status = handle_command_word(state, &cursor, command);
		if (status)
			return (status);
	}
	return (set_command_executable(state, command));
}
