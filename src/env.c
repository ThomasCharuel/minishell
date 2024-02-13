/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 17:17:41 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/13 17:35:39 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	minishell_env(t_state *state, int argc, char **argv)
{
	size_t	i;

	(void)argc;
	(void)argv;
	if (argc > 1)
		return (print_error("env: too many arguments", NULL),
			COMMAND_TOO_MANY_ARGUMENTS);
	i = 0;
	while (state->envp && state->envp[i])
		ft_printf("%s\n", state->envp[i++]);
	return (COMMAND_SUCCESS);
}

t_command_status	minishell_export(t_state *state, int argc, char **argv)
{
	(void)state;
	(void)argc;
	(void)argv;
	return (COMMAND_SUCCESS);
}

t_command_status	minishell_unset(t_state *state, int argc, char **argv)
{
	(void)state;
	(void)argc;
	(void)argv;
	return (COMMAND_SUCCESS);
}
