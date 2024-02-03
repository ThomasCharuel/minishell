/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 20:49:59 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 21:03:40 by tcharuel         ###   ########.fr       */
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
