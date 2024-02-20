/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:12:35 by rdupeux           #+#    #+#             */
/*   Updated: 2024/02/20 13:12:50 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	minishell_pwd(t_state *state, int argc, char **argv)
{
	char *pwd;

	(void)state;
	(void)argc;
	(void)argv;
	pwd = get_working_directory();
	if (!pwd)
		return (COMMAND_ERROR);
	if (*pwd)
		ft_printf("%s\n", pwd);
	else
		print_error("pwd: error: cannot determine current directory: No such file or directory",
			NULL);
	free(pwd);
	return (COMMAND_SUCCESS);
}