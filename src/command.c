/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 20:49:59 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/09 20:19:59 by tcharuel         ###   ########.fr       */
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
	command->in_fd = STDIN_FILENO;
	command->out_fd = STDOUT_FILENO;
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
	if (ft_strchr((char *)command->argv->content, '/'))
		status = handle_path_command(command);
	else
		status = handle_command(state, command);
	return (COMMAND_SUCCESS);
}

t_command_status	command_parse(t_state *state, t_command *command)
{
	char				*word;
	const char			*cursor;
	t_command_status	status;

	cursor = command->command_str;
	while (*cursor)
	{
		while (*cursor == ' ')
			cursor++;
		if (!*cursor)
			break ;
		if (ft_is_char_in_set(*cursor, "<>"))
		{
			status = handle_redirection(state, &cursor, command);
			if (status)
				return (status);
		}
		else
		{
			status = get_next_word(&cursor, &word);
			if (status)
				return (status);
			if (!word)
				break ;
			status = handle_word_interpretation(state, &word);
			if (status)
				return (status);
			if (!word)
				break ;
			if (!str_list_append(&command->argv, word))
				return (free(word), COMMAND_ERROR);
		}
	}
	// command_display(command);
	return (set_command_command(state, command));
}
