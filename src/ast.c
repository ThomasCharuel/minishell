/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:37:41 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/18 16:02:16 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_node(t_state *state, t_node *node)
{
	(void)state;
	if (node->type == COMMAND)
		ft_printf("command node: %s, daddy: %p\n",
			((t_command *)node->content)->command_str, node->daddy);
	if (node->type == PIPE)
		ft_printf("PIPE\n");
	if (node->type == AND)
		ft_printf("AND\n");
	if (node->type == OR)
		ft_printf("OR\n");
}

t_command_status	and_execute(t_state *state, t_node *node)
{
	t_command_status	status;
	int					command_status;

	status = ast_execute(state, node->left);
	if (status)
		return (status);
	if (state->last_child_pid)
	{
		waitpid(state->last_child_pid, &command_status, 0);
		state->last_exit_code = WIFEXITED(command_status)
			&& WEXITSTATUS(command_status);
	}
	if (state->last_exit_code)
		return (state->last_exit_code);
	return (ast_execute(state, node->right));
}

t_command_status	or_execute(t_state *state, t_node *node)
{
	t_command_status	status;
	int					command_status;

	status = ast_execute(state, node->left);
	if (status)
		return (ast_execute(state, node->right));
	if (state->last_child_pid)
	{
		waitpid(state->last_child_pid, &command_status, 0);
		state->last_exit_code = WIFEXITED(command_status)
			&& WEXITSTATUS(command_status);
		if (state->last_exit_code)
			return (ast_execute(state, node->right));
	}
	return (COMMAND_SUCCESS);
}

t_command_status	pipe_execute(t_state *state, t_node *node)
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

t_command_status	command_execute(t_state *state, t_node *node)
{
	t_command_status	status;

	status = command_parse(state, node->content);
	state->last_exit_code = status;
	if (status)
		return (status);
	status = command_exec(state, node);
	state->last_exit_code = status;
	return (status);
}

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
