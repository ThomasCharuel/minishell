/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:23:44 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 14:05:49 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(const char *str)
{
	char	*builtins[7];
	int		i;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	i = 0;
	while (i < 7)
		if (!ft_strcmp(str, builtins[i++]))
			return (true);
	return (false);
}

static void	fd_setup_builtin(t_node *node)
{
	if (node->daddy && node->daddy->type == PIPE)
	{
		if (node->write_fd != STDOUT_FILENO)
		{
			dup2(node->write_fd, STDOUT_FILENO);
			close(node->write_fd);
		}
		if (node->write_fd != STDIN_FILENO)
		{
			dup2(node->read_fd, STDIN_FILENO);
			close(node->read_fd);
		}
	}
}

t_command_status	builtin_exec_decorator(t_state *state, t_node *node,
		char **argv, t_command_status (*builtin)(t_state *, int, char **))
{
	int					stdout_;
	int					stdin_;
	int					argc;
	t_command_status	status;

	argc = 0;
	while (argv[argc])
		argc++;
	stdout_ = dup(STDOUT_FILENO);
	stdin_ = dup(STDIN_FILENO);
	fd_setup_builtin(node);
	handle_redirections(node);
	status = builtin(state, argc, argv);
	state->last_exit_code = status;
	dup2(stdin_, STDIN_FILENO);
	close(stdin_);
	dup2(stdout_, STDOUT_FILENO);
	close(stdout_);
	free(argv);
	return (status);
}

t_command_status	builtin_exec(t_state *state, t_node *node, char **argv)
{
	if (!ft_strcmp(argv[0], "echo"))
		return (builtin_exec_decorator(state, node, argv, &minishell_echo));
	else if (!ft_strcmp(argv[0], "cd"))
		return (builtin_exec_decorator(state, node, argv, &minishell_cd));
	else if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_exec_decorator(state, node, argv, &minishell_pwd));
	else if (!ft_strcmp(argv[0], "export"))
		return (builtin_exec_decorator(state, node, argv, &minishell_export));
	else if (!ft_strcmp(argv[0], "unset"))
		return (builtin_exec_decorator(state, node, argv, &minishell_unset));
	else if (!ft_strcmp(argv[0], "env"))
		return (builtin_exec_decorator(state, node, argv, &minishell_env));
	else if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exec_decorator(state, node, argv, &minishell_exit));
	return (COMMAND_PARSING_ERROR);
}
