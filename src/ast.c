/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:37:41 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/13 20:02:47 by tcharuel         ###   ########.fr       */
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

t_command_status	ast_execute(t_state *state, t_node *node)
{
	t_command_status	status;
	int					command_status;

	if (node->type == AND)
	{
		status = ast_execute(state, node->left);
		if (status)
			return (status);
		waitpid(state->last_child_pid, &command_status, 0);
		state->last_exit_code = WIFEXITED(command_status)
			&& WEXITSTATUS(command_status);
		if (state->last_exit_code)
			return (state->last_exit_code);
		return (ast_execute(state, node->right));
	}
	else if (node->type == OR)
	{
		status = ast_execute(state, node->left);
		if (status)
		{
			waitpid(state->last_child_pid, &command_status, 0);
			state->last_exit_code = WIFEXITED(command_status)
				&& WEXITSTATUS(command_status);
			if (!state->last_exit_code)
				return (COMMAND_SUCCESS);
		}
		return (ast_execute(state, node->right));
	}
	else if (node->type == PIPE)
	{
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
	status = command_parse(state, node->content);
	if (status)
		return (status);
	return (command_exec(state, node));
}
