/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 17:17:41 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/16 14:23:33 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	minishell_env(t_state *state, int argc, char **argv)
{
	size_t	i;

	(void)argv;
	if (argc > 1)
		return (print_error("env: too many arguments", NULL),
			COMMAND_TOO_MANY_ARGUMENTS);
	i = 0;
	while (state->envp && state->envp[i])
	{
		if (strchr(state->envp[i], '='))
			ft_printf("%s\n", state->envp[i]);
		i++;
	}
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

t_command_status	parse_export_statement(const char *str, char **key,
		char **value)
{
	size_t	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (print_error("export: ", str, ": not a valid identifier", NULL),
			COMMAND_PARSING_ERROR);
	i = 1;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (print_error("export: ", str, ": not a valid identifier",
					NULL), COMMAND_PARSING_ERROR);
		i++;
	}
	*key = ft_strndup(str, i);
	if (!*key)
		return (COMMAND_ERROR);
	if (str[i] != '=')
		return (*value = NULL, COMMAND_SUCCESS);
	*value = (char *)&str[i + 1];
	return (COMMAND_SUCCESS);
}

t_command_status	minishell_export(t_state *state, int argc, char **argv)
{
	t_command_status	status;
	char				*key;
	char				*value;
	int					i;

	key = NULL;
	if (argc == 1)
		return (print_declare_statements(state->envp));
	else
	{
		i = 0;
		while (++i < argc)
		{
			status = parse_export_statement(argv[i], &key, &value);
			if (status)
				return (status);
			if (!envp_set(state, key, value))
				return (free(key), COMMAND_ERROR);
			free(key);
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
