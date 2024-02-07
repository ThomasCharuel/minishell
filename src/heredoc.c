/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:13:26 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/07 17:04:02 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	handle_heredocs(t_command *command)
{
	char *cursor;
	char *new_cursor;

	return (COMMAND_SUCCESS);
	cursor = command->command_str;
	new_cursor = ft_strchr(cursor, '<');
	if (!new_cursor)
		return (COMMAND_SUCCESS);
}