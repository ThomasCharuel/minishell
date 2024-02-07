/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:34:15 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/07 17:03:22 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*redirection_create(const char *file, t_redirection_type type)
{
	t_redirection	*redirection;

	redirection = malloc(sizeof(t_redirection));
	if (!redirection)
		return (perror("minishell"), NULL);
	redirection->file = ft_strdup(file);
	if (!redirection->file)
		return (perror("minishell"), free(redirection), NULL);
	redirection->type = type;
	return (redirection);
}

void	redirection_destroy(void *ptr)
{
	t_redirection	*redirection;

	redirection = ptr;
	if (redirection)
	{
		if (redirection->file)
			free(redirection->file);
		free(redirection);
	}
}

t_command_status	handle_redirection(t_state *state, const char **cmd,
		t_command *command)
{
	const char			*cursor;
	char				*file;
	t_redirection		*redirection;
	t_redirection_type	type;
	t_command_status	status;

	cursor = *cmd;
	if (*cursor == '>')
	{
		if (*(cursor + 1) == '>')
		{
			cursor++;
			type = APPEND;
		}
		else
			type = WRITE;
	}
	else
		type = READ;
	cursor++;
	while (*cursor == ' ')
		cursor++;
	if (!*cursor || ft_is_char_in_set(*cursor, "<>"))
		return (COMMAND_PARSING_ERROR);
	status = get_next_word(&cursor, &file);
	if (status != COMMAND_SUCCESS)
		return (status);
	status = handle_word_interpretation(state, &file);
	redirection = redirection_create(file, type);
	free(file);
	if (!redirection || !ft_append(&command->redirections, redirection))
		return (COMMAND_ERROR);
	*cmd = cursor;
	return (COMMAND_SUCCESS);
}
