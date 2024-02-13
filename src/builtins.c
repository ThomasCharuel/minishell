/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:23:44 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/13 16:20:24 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac == 1)
		write(1, "\n", 1);
	if (ac >= 2 && !ft_strncmp("-n", av[1], 2))
	{
		i++;
		while (++i < ac)
		{
			write(1, av[i], ft_strlen(av[i]));
			if (i + 1 < ac)
				write(1, " ", 1);
		}
		return (0);
	}
	if (ac >= 2)
		while (++i < ac)
		{
			write(1, av[i], ft_strlen(av[i]));
			if (i + 1 < ac)
				write(1, " ", 1);
		}
	write(1, "\n", 1);
	return (0);
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
	while (argv[argc++])
		;
	stdout_ = dup(STDOUT_FILENO);
	stdin_ = dup(STDIN_FILENO);
	//	if (node->daddy->type == pipe)
	//	{
	//		dup2()
	//	}
	handle_redirections(node);
	status = builtin(state, argc, argv);
	state->last_exit_code = status;
	dup2(stdin_, STDIN_FILENO);
	dup2(stdout_, STDOUT_FILENO);
	return (status);
}

t_command_status	builtin_exec(t_state *state, t_node *node, char **argv)
{
	if (!ft_strcmp(argv[0], "echo"))
		return (COMMAND_SUCCESS);
	else if (!ft_strcmp(argv[0], "cd"))
		return (COMMAND_SUCCESS);
	else if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_exec_decorator(state, node, argv, &minishell_pwd));
	else if (!ft_strcmp(argv[0], "export"))
		return (COMMAND_SUCCESS);
	else if (!ft_strcmp(argv[0], "unset"))
		return (COMMAND_SUCCESS);
	else if (!ft_strcmp(argv[0], "env"))
		return (COMMAND_SUCCESS);
	else if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exec_decorator(state, node, argv, &minishell_exit));
	return (COMMAND_PARSING_ERROR);
}
