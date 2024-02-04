/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 20:49:59 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/04 20:34:09 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*command_create(const char *command_str)
{
	t_command	*command;

	command = (t_command *)malloc(sizeof(t_command));
	if (!command)
		return (perror("minishell"), NULL);
	command->argv = ft_split(command_str, ' ');
	if (!command->argv)
		return (perror("minishell"), free(command), NULL);
	command->fd[IN_FD] = STDIN_FILENO;
	command->fd[OUT_FD] = STDOUT_FILENO;
	return (command);
}

void	command_destroy(t_command **command)
{
	if (*command)
	{
		ft_free_strs((*command)->argv);
		free(*command);
		*command = NULL;
	}
}

t_command_status	command_parse(t_state *state, t_command *command)
{
	t_command_status	status;

	if (!command->argv || !command->argv[0])
		status = COMMAND_NOT_FOUND;
	else if (ft_strchr(command->argv[0], '/'))
		status = handle_path_command(command);
	else
		status = handle_command(state, command);
	return (status);
}
