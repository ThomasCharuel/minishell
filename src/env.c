/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 17:17:41 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/13 18:29:54 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	minishell_env(t_state *state, int argc, char **argv)
{
	size_t	i;

	// N'affiche pas truc vide
	(void)argv;
	if (argc > 1)
		return (print_error("env: too many arguments", NULL),
			COMMAND_TOO_MANY_ARGUMENTS);
	i = 0;
	while (state->envp && state->envp[i])
		ft_printf("%s\n", state->envp[i++]);
	return (COMMAND_SUCCESS);
}

t_command_status	print_declare_statements(char **envp)
{
	size_t	i;
	char	*declare_statement;
	char	**strs;

	i = 0;
	while (envp[i])
	{
		strs = ft_split(envp[i], '=');
		if (!strs)
			return (COMMAND_ERROR);
		if (strs[1])
			declare_statement = ft_strsjoin("declare -x ", strs[0], "=\"",
					strs[1], "\"", NULL);
		else
			declare_statement = ft_strsjoin("declare -x ", strs[0], NULL);
		ft_clean_double_list((void **)strs, free);
		if (!declare_statement)
			return (COMMAND_ERROR);
		ft_printf("%s\n", declare_statement);
		i++;
	}
	return (COMMAND_SUCCESS);
}

t_command_status	minishell_export(t_state *state, int argc, char **argv)
{
	char	*key;
	char	*value;
	int		i;

	if (argc == 1)
		return (print_declare_statements(state->envp));
	else
	{
		i = 0;
		while (++i < argc)
		{
			value = ft_strchr(argv[i], '=');
			if (!value)
				return (envp_set(state, argv[i], NULL));
			else
			{
				if (!envp_set(state, key, value))
					return (COMMAND_ERROR);
			}
		}
	}
	return (COMMAND_SUCCESS);
}

t_command_status	minishell_unset(t_state *state, int argc, char **argv)
{
	size_t	i;

	(void)argc;
	i = 1;
	while (argv[i])
		envp_delete(state, argv[i++]);
	return (COMMAND_SUCCESS);
}
