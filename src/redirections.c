/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:11:32 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 14:44:05 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static t_command_status	handle_write_redirection(t_node *node,
		t_redirection *redirection)
{
	if (redirection->type == WRITE)
		node->write_fd = open(redirection->file, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else if (redirection->type == APPEND)
		node->write_fd = open(redirection->file, O_WRONLY | O_CREAT | O_APPEND,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (node->write_fd < 0)
		return (perror("minishell"), COMMAND_TOO_MANY_ARGUMENTS);
	if (dup2(node->write_fd, STDOUT_FILENO) == -1)
		return (perror("minishell"), COMMAND_ERROR);
	ft_close_fd(node->write_fd);
	return (COMMAND_SUCCESS);
}

static t_command_status	file_interpreteur(t_state *state, int fd1, int fd2)
{
	char	*line;
	char	*cursor;
	char	*word;

	line = get_next_line(fd1);
	while (line)
	{
		cursor = ft_strchr(line, '$');
		if (!cursor)
			write(fd2, line, ft_strlen(line));
		else
		{
			write(fd2, line, cursor - line);
			cursor = &line[cursor - line + 1];
			get_var_value(state, &cursor, &word);
			write(fd2, word, ft_strlen(word));
			write(fd2, cursor, ft_strlen(cursor));
			ft_free_str(&word);
		}
		ft_free_str(&line);
		line = get_next_line(fd1);
	}
}

// TODO
static t_command_status	handle_read_redirection(t_node *node,
		t_redirection *redirection)
{
	node->read_fd = open(redirection->file, O_RDONLY);
	if (node->read_fd < 0)
		return (perror("minishell"), COMMAND_TOO_MANY_ARGUMENTS);
	if (!ft_strcmp("/tmp/.minishell-hi-", redirection->file))
		file_interpreteur(state, node->read_fd, STDIN_FILENO);
	// Handle interpretation des heredocs
	// En fonction du nom du fichier, ecrire dans node->read_fd
	else
	{
		if (dup2(node->read_fd, STDIN_FILENO) == -1)
			return (perror("minishell"), COMMAND_ERROR);
		ft_close_fd(node->read_fd);
	}
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	handle_redirections(t_node *node)
{
	t_command_status	status;
	t_list				*argv_node;
	t_redirection		*redirection;
	t_command			*command;

	command = node->content;
	argv_node = command->redirections;
	while (argv_node)
	{
		redirection = argv_node->content;
		if (redirection->type == WRITE || redirection->type == APPEND)
			status = handle_write_redirection(node, redirection);
		else if (redirection->type == READ)
			status = handle_read_redirection(node, redirection);
		if (status)
			return (status);
		argv_node = argv_node->next;
	}
	return (COMMAND_SUCCESS);
}
