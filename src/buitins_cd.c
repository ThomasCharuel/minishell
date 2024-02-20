/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:11:53 by rdupeux           #+#    #+#             */
/*   Updated: 2024/02/20 13:51:46 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	minishell_cd(t_state *state, int argc, char **argv)
{
	const char	*path;

	if (argc == 1)
	{
		path = envp_get(state, "HOME");
		if (!*path)
			return (print_error("cd: HOME not set", NULL),
				COMMAND_TOO_MANY_ARGUMENTS);
	}
	else if (argc == 2)
		path = argv[1];
	else
		return (print_error("cd: too many arguments", NULL),
			COMMAND_TOO_MANY_ARGUMENTS);
	if (chdir(path))
		return (perror("minishell"), COMMAND_TOO_MANY_ARGUMENTS);
	return (COMMAND_SUCCESS);
}
