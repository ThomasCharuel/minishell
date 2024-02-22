/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:12:35 by rdupeux           #+#    #+#             */
/*   Updated: 2024/02/22 11:49:00 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	minishell_pwd(t_state *state, int argc, char **argv)
{
	char	*pwd;

	(void)state;
	if (argc > 1 && *argv[1] == '-' && argv[1][1])
		return (print_error("pwd: ", argv[1], ": invalid option",
				"\npwd: usage: pwd", NULL), COMMAND_PARSING_ERROR);
	pwd = get_working_directory();
	if (!pwd)
		return (COMMAND_ERROR);
	if (*pwd)
		ft_printf("%s\n", pwd);
	else
	{
		print_error("pwd: error: cannot determine current directory:",
			"No such file or directory", NULL);
	}
	free(pwd);
	return (COMMAND_SUCCESS);
}
