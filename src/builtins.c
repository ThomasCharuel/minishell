/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:23:44 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/13 19:10:28 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	minishell_echo(t_state *state, int argc, char **argv)
{
	int		i;
	bool	should_print_new_line;

	(void)state;
	should_print_new_line = true;
	i = 1;
	if (argc > 1 && !ft_strcmp("-n", argv[i]))
	{
		should_print_new_line = false;
		i++;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i++]);
		if (i < argc)
			ft_printf(" ");
	}
	if (should_print_new_line)
		ft_printf("\n");
	return (COMMAND_SUCCESS);
}

int	cd(int ac, char **av)
{
	char	*home;

	home = getenv("HOME"); // To refacto
	if (ac == 1)
		return (chdir(home));
	if (ac > 2)
	{
		write(2, "too many arguments", 19);
	}
	else if (chdir(av[1]))
		perror(av[1]);
	return (1);
}

t_command_status	minishell_pwd(t_state *state, int argc, char **argv)
{
	char	*wd;

	(void)argc;
	(void)argv;
	(void)state;
	wd = malloc(PATH_MAX * sizeof(char));
	if (!wd)
		return (COMMAND_ERROR);
	getcwd(wd, PATH_MAX);
	write(STDOUT_FILENO, wd, ft_strlen(wd));
	write(STDOUT_FILENO, "\n", 1);
	free(wd);
	return (COMMAND_SUCCESS);
}

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
	if (node->daddy && node->daddy->type == PIPE)
	{
		dup2(node->write_fd, STDOUT_FILENO);
		close(node->write_fd);
		dup2(node->read_fd, STDIN_FILENO);
		close(node->read_fd);
	}
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
		return (COMMAND_SUCCESS);
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
