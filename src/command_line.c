/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:16:15 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 00:09:33 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static const char	*move_to_next_char(const char **ptr, char c)
{
	const char	*cursor;

	cursor = ft_strchr((*ptr) + 1, c);
	if (cursor)
		*ptr = cursor;
	return (cursor);
}

/**
 * Runs a initial check of the validity of the input line
 * for proper closure of quotes,
 * double quotes, and parentheses.
 */
static t_command_status	command_line_validity_check(const char *line)
{
	int	open_parenthesis;

	open_parenthesis = 0;
	while (line && *line)
	{
		if (*line == '(')
			open_parenthesis++;
		else if (*line == ')' && open_parenthesis-- < 1)
			return (print_error("syntax error near unexpected token `)'", NULL),
				COMMAND_PARSING_ERROR);
		else if (ft_is_char_in_set(*line, "\"\'") && !move_to_next_char(&line,
				*line))
		{
			if (*line == '\'')
				return (print_error("syntax error: unclosed quote", NULL),
					COMMAND_PARSING_ERROR);
			return (print_error("syntax error: unclosed double quotes", NULL),
				COMMAND_PARSING_ERROR);
		}
		line = ft_strchrs(line + 1, "\'\"()");
	}
	if (open_parenthesis)
		return (print_error("syntax error: unclosed parenthesis", NULL),
			COMMAND_PARSING_ERROR);
	return (COMMAND_SUCCESS);
}

// OK
static t_command_status	command_line_parse(t_state *state, const char *line)
{
	t_command_status	status;
	const char			*str;

	status = command_line_validity_check(line);
	if (status)
		return (status);
	str = line;
	status = handle_word(state, &str, &state->line, &handle_heredocs);
	if (status)
		return (status);
	status = ast_generate(state);
	ft_free_str(&state->line);
	return (status);
}

// OK
t_command_status	command_line_execute(t_state *state, const char *line)
{
	int					command_status;
	t_command_status	status;

	state->last_child_pid = 0;
	status = command_line_parse(state, line);
	if (status)
		return (state->last_exit_code = status, status);
	status = ast_execute(state, state->ast);
	if (!status && state->last_child_pid)
	{
		waitpid(state->last_child_pid, &command_status, 0);
		status = !WIFEXITED(command_status);
		if (!status)
			status = WEXITSTATUS(command_status);
	}
	state->last_exit_code = status;
	while (wait(NULL) != -1)
		continue ;
	node_destroy(&state->ast);
	ft_lstclear(&state->heredocs, &heredoc_destroy);
	return (status);
}
