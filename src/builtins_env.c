/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:11:40 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 12:14:31 by tcharuel         ###   ########.fr       */
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

t_command_status	minishell_unset(t_state *state, int argc, char **argv)
{
	size_t	i;

	(void)argc;
	i = 1;
	while (argv[i])
		envp_delete(state, argv[i++]);
	return (COMMAND_SUCCESS);
}
