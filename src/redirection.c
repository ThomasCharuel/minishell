/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:34:15 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 10:33:23 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*redirection_create(const char *file, t_redirection_type type)
{
	t_redirection	*redirection;

	redirection = malloc(sizeof(t_redirection));
	if (!redirection)
		return (perror("minishell"), NULL);
	redirection->file = ft_strdup(file);
	if (!redirection->file)
		return (perror("minishell"), free(redirection), NULL);
	redirection->type = type;
	return (redirection);
}

void	redirection_destroy(void *ptr)
{
	t_redirection	*redirection;

	redirection = ptr;
	if (redirection)
	{
		if (redirection->file)
			free(redirection->file);
		free(redirection);
	}
}

t_command_status	handle_redirection(t_state *state, const char **cmd,
		t_command *command)
{
	const char			*cursor;
	char				*file;
	t_redirection		*redirection;
	t_redirection_type	type;
	t_command_status	status;

	cursor = *cmd;
	if (*cursor == '>')
	{
		if (*(cursor + 1) == '>')
		{
			cursor++;
			type = APPEND;
		}
		else
			type = WRITE;
	}
	else
		type = READ;
	cursor++;
	while (*cursor == ' ')
		cursor++;
	if (!*cursor || ft_is_char_in_set(*cursor, "<>"))
		return (COMMAND_PARSING_ERROR);
	status = handle_word(state, &cursor, &file, &get_next_token);
	if (status)
		return (status);
	status = handle_word_interpretation(state, &file);
	if (status)
		return (status);
	status = suppr_quotes(&file);
	if (status)
		return (status);
	redirection = redirection_create(file, type);
	free(file);
	if (!redirection || !ft_append(&command->redirections, redirection))
		return (COMMAND_ERROR);
	*cmd = cursor;
	return (COMMAND_SUCCESS);
}

void	handle_redirections(t_node *node)
{
	t_list			*argv_node;
	t_redirection	*redirection;
	t_command		*command;

	command = node->content;
	argv_node = command->redirections;
	while (argv_node)
	{
		redirection = argv_node->content;
		if (redirection->type == WRITE || redirection->type == APPEND)
		{
			if (redirection->type == WRITE)
				node->write_fd = open(redirection->file,
						O_WRONLY | O_CREAT | O_TRUNC,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			else if (redirection->type == APPEND)
				node->write_fd = open(redirection->file,
						O_WRONLY | O_CREAT | O_APPEND,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(node->write_fd, STDOUT_FILENO);
			ft_close_fd(node->write_fd);
		}
		else if (redirection->type == READ)
		{
			node->read_fd = open(redirection->file, O_RDONLY);
			dup2(node->read_fd, STDIN_FILENO);
			ft_close_fd(node->read_fd);
		}
		argv_node = argv_node->next;
	}
}
