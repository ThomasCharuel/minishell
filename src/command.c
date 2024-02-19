/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 20:49:59 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 00:42:48 by tcharuel         ###   ########.fr       */
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

t_command_status	handle_subshell(t_state *state, t_command *command,
		const char **cursor)
{
	char				*word;
	char				*res;
	t_command_status	status;

	word = ft_strdup(state->executable_path);
	if (!word)
		return (COMMAND_ERROR);
	if (!str_list_append(&command->argv, word))
		return (COMMAND_ERROR);
	status = handle_word(state, cursor, &res, &get_next_parenthesis_expression);
	if (status)
		return (status);
	word = ft_strndup(&res[1], ft_strlen(res) - 3);
	if (!word)
		return (free(res), COMMAND_ERROR);
	if (!str_list_append(&command->argv, word))
		return (free(res), free(word), COMMAND_ERROR);
	free(res);
	return (COMMAND_SUCCESS);
}

t_command_status	command_parse(t_state *state, t_command *command)
{
	char				*word;
	const char			*cursor;
	t_command_status	status;
	bool				first_run;

	first_run = true;
	cursor = command->command_str;
	while (*cursor)
	{
		while (*cursor == ' ')
			cursor++;
		if (!*cursor)
			break ;
		if (*cursor == '(')
		{
			if (!first_run)
				return (print_error("syntax error: unexpected parenthesis",
						NULL), COMMAND_PARSING_ERROR);
			status = handle_subshell(state, command, &cursor);
			if (status)
				return (status);
		}
		else if (ft_is_char_in_set(*cursor, "<>"))
		{
			status = handle_redirection(state, &cursor, command);
			if (status)
				return (status);
		}
		else
		{
			status = handle_word(state, &cursor, &word, &get_next_token);
			if (status)
				return (status);
			if (!word)
				break ;
			status = handle_word_interpretation(state, &word);
			if (status)
				return (status);
			if (!word)
				break ;
			if (!first_run)
			{
				status = handle_wildecards(state, &word, command->argv);
				if (!word)
					continue ;
				if (status)
					return (status);
			}
			status = suppr_quotes(&word);
			if (status)
				return (status);
			if (!str_list_append(&command->argv, word))
				return (free(word), COMMAND_ERROR);
		}
		first_run = false;
	}
	return (set_command_executable(state, command));
}
