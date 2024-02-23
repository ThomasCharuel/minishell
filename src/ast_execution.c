/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:37:41 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 10:11:21 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static t_command_status	and_execute(t_state *state, t_node *node)
{
	t_command_status	status;
	int					command_status;

	status = ast_execute(state, node->left);
	if (!status && state->last_child_pid)
	{
		waitpid(state->last_child_pid, &command_status, 0);
		status = !WIFEXITED(command_status);
		if (!status)
			status = WEXITSTATUS(command_status);
	}
	state->last_exit_code = status;
	if (status)
		return (status);
	while (wait(NULL) != -1)
		continue ;
	return (ast_execute(state, node->right));
}

// OK
static t_command_status	or_execute(t_state *state, t_node *node)
{
	t_command_status	status;
	int					command_status;

	status = ast_execute(state, node->left);
	if (!status && state->last_child_pid)
	{
		waitpid(state->last_child_pid, &command_status, 0);
		status = !WIFEXITED(command_status);
		if (!status)
			status = WEXITSTATUS(command_status);
	}
	state->last_exit_code = status;
	while (wait(NULL) != -1)
		continue ;
	if (status)
		return (ast_execute(state, node->right));
	return (COMMAND_SUCCESS);
}

// OK
static t_command_status	pipe_execute(t_state *state, t_node *node)
{
	t_command_status	status;

	node->content = pipe_create();
	if (!node->content)
		return (COMMAND_ERROR);
	node->left->read_fd = node->read_fd;
	node->left->write_fd = ((t_pipe *)node->content)->fd[OUT_FD];
	node->right->read_fd = ((t_pipe *)node->content)->fd[IN_FD];
	status = ast_execute(state, node->left);
	if (status)
		return (status);
	return (ast_execute(state, node->right));
}

// OK
static t_command_status	command_execute(t_state *state, t_node *node)
{
	t_command_status	status;

	status = command_parse(state, node->content);
	if (!status)
		status = command_exec(state, node);
	state->last_exit_code = status;
	return (status);
}

// OK
t_command_status	ast_execute(t_state *state, t_node *node)
{
	if (node->type == AND)
		return (and_execute(state, node));
	else if (node->type == OR)
		return (or_execute(state, node));
	else if (node->type == PIPE)
		return (pipe_execute(state, node));
	else if (node->daddy && node->daddy->type == PIPE)
		return (subshell_execute(state, node));
	return (command_execute(state, node));
}
