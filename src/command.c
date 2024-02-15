/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 20:49:59 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/15 16:12:02 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	command_display(t_command *command)
{
	t_list			*node;
	t_redirection	*redirection;

	node = command->argv;
	while (node)
	{
		ft_printf("Argv: \"%s\"\n", node->content);
		node = node->next;
	}
	node = command->redirections;
	while (node)
	{
		redirection = node->content;
		ft_printf("Redirections: \"%s\" (%d)\n", redirection->file,
			redirection->type);
		node = node->next;
	}
}

t_command_status	set_command_command(t_state *state, t_command *command)
{
	t_command_status	status;

	if (!command->argv)
		return (COMMAND_NOT_FOUND);
	if (is_builtin(command->argv->content))
		return (COMMAND_SUCCESS);
	if (ft_strchr((char *)command->argv->content, '/'))
		status = handle_path_command(command);
	else
		status = handle_command(state, command);
	(void)status; // handle status
	return (COMMAND_SUCCESS);
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
	status = get_func_decorator(cursor, &res, &get_next_parenthesis_expression);
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
		if (first_run && *cursor == '(')
		{
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
			status = get_func_decorator(&cursor, &word, &get_next_token);
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
	return (set_command_command(state, command));
}
