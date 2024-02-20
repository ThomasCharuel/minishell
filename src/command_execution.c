/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 11:28:54 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static int	get_fd_to_close(t_node *node)
{
	t_node	*daddy;

	daddy = node->daddy;
	if (daddy && daddy->type == PIPE && daddy->right != node)
		return (((t_pipe *)daddy->content)->fd[IN_FD]);
	return (STDIN_FILENO);
}

// OK
static void	child_exec(t_state *state, t_node *node, char **argv)
{
	t_command_status	status;

	ft_close_fd(get_fd_to_close(node));
	dup2(node->read_fd, STDIN_FILENO);
	ft_close_fd(node->read_fd);
	dup2(node->write_fd, STDOUT_FILENO);
	ft_close_fd(node->write_fd);
	status = handle_redirections(node);
	if (!status && execve(argv[0], argv, state->envp) == -1)
	{
		perror("minishell");
		status = COMMAND_ERROR;
	}
	free(argv);
	state_cleanup(state);
	exit(status);
}

// OK
t_command_status	command_exec(t_state *state, t_node *node)
{
	char		**argv;
	t_command	*command;

	command = node->content;
	argv = get_strs_from_list(command->argv);
	if (!argv)
		return (perror("minishell"), COMMAND_ERROR);
	if (is_builtin(argv[0]))
		return (state->last_child_pid = 0, builtin_exec(state, node, argv));
	state->last_child_pid = fork();
	if (state->last_child_pid == -1)
		return (perror("minishell"), COMMAND_ERROR);
	if (state->last_child_pid == 0)
		child_exec(state, node, argv);
	ft_close_fd(node->read_fd);
	ft_close_fd(node->write_fd);
	free(argv);
	return (COMMAND_SUCCESS);
}
