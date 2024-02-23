/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:59:49 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 14:11:01 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	move(const char **line, bool *is_operator)
{
	while (**line == ' ')
		(*line)++;
	if (**line && !ft_is_char_in_set(**line, "|&"))
		*is_operator = false;
	if (ft_is_char_in_set(**line, "\"\'"))
	{
		*line = move_to_next_char(line, **line);
		if (*line)
			(*line)++;
	}
}

t_command_status	check_subcommand(const char *line)
{
	bool	is_operator;

	is_operator = false;
	while (line && *line)
	{
		if (!ft_strncmp(line, "&&", 2) || !ft_strncmp(line, "||", 2)
			|| *line == '|')
		{
			if (is_operator)
				break ;
			is_operator = true;
			if (!ft_strncmp(line, "&&", 2) || !ft_strncmp(line, "||", 2))
				line += 2;
			else
				line++;
		}
		move(&line, &is_operator);
		line = ft_strchrs(line, "&|\'\"");
	}
	if (is_operator)
		return (print_error("syntax error near unexpected token", NULL),
			COMMAND_PARSING_ERROR);
	return (COMMAND_SUCCESS);
}

static bool	is_empty_parenthesis(const char *line)
{
	line++;
	while (*line && *line == ' ')
		line++;
	if (!*line || *line == ')')
		return (true);
	return (false);
}

t_command_status	check_empty_parenthesis(const char *line)
{
	while (line && *line)
	{
		if (*line == '(' && is_empty_parenthesis(line))
			return (print_error("syntax error near unexpected token `)'", NULL),
				COMMAND_PARSING_ERROR);
		if (ft_is_char_in_set(*line, "\"\'") && !move_to_next_char(&line,
				*line))
			break ;
		line = ft_strchrs(line + 1, "\'\"(");
	}
	return (COMMAND_SUCCESS);
}
