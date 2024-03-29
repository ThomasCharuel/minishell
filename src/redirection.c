/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:34:15 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 17:13:06 by tcharuel         ###   ########.fr       */
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
		ft_free_str(&redirection->file);
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

t_command_status	handle_redirection_file(t_state *state, const char **cursor,
		char **file, t_redirection_type type)
{
	t_command_status	status;

	status = handle_word(NULL, cursor, file, &get_next_token);
	if (status)
		return (status);
	status = handle_word(state, (const char **)file, file, &handle_env_var);
	if (status)
		return (ft_free_str(file), status);
	status = handle_word(NULL, (const char **)file, file, &remove_quotes);
	if (status)
		return (ft_free_str(file), status);
	if (type == READ && access(*file, R_OK) == -1)
		return (print_error(*file, ": No such file or directory", NULL),
			ft_free_str(file), COMMAND_REDIRECTION_ERROR);
	return (COMMAND_SUCCESS);
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
	status = handle_redirection_file(state, cursor, &file, type);
	if (status)
		return (status);
	redirection = redirection_create(file, type);
	if (!redirection)
		return (ft_free_str(&file), COMMAND_ERROR);
	if (!ft_append(&command->redirections, redirection))
		return (redirection_destroy(redirection), COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}
