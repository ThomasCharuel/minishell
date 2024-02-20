/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:34:15 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 19:18:56 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static t_redirection	*redirection_create(char *file, t_redirection_type type)
{
	t_redirection	*redirection;

	redirection = malloc(sizeof(t_redirection));
	if (!redirection)
		return (perror("minishell"), NULL);
	redirection->file = file;
	redirection->type = type;
	return (redirection);
}

// OK
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

// OK
static t_redirection_type	get_redirection_type(const char **cursor)
{
	t_redirection_type	type;

	if (**cursor == '>')
	{
		if (*(*cursor + 1) == '>')
		{
			(*cursor)++;
			type = APPEND;
		}
		else
			type = WRITE;
	}
	else
		type = READ;
	(*cursor)++;
	return (type);
}

// OK
t_command_status	handle_redirection(t_state *state, const char **cursor,
		t_command *command)
{
	char				*file;
	t_redirection		*redirection;
	t_redirection_type	type;
	t_command_status	status;

	type = get_redirection_type(cursor);
	while (**cursor == ' ')
		(*cursor)++;
	if (!**cursor || ft_is_char_in_set(**cursor, "<>"))
		return (print_error("syntax error", NULL), COMMAND_PARSING_ERROR);
	status = handle_word(NULL, cursor, &file, &get_next_token);
	if (status)
		return (status);
	status = handle_word(state, (const char **)&file, &file, &handle_env_var);
	if (status)
		return (status);
	status = handle_word(NULL, (const char **)&file, &file, &remove_quotes);
	if (status)
		return (status);
	redirection = redirection_create(file, type);
	if (!redirection)
		return (free(file), COMMAND_ERROR);
	if (!ft_append(&command->redirections, redirection))
		return (redirection_destroy(redirection), COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}
