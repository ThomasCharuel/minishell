/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:21:40 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/15 20:06:41 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	get_next_word_new(const char **cursor, char **res,
		const char *charset, bool delim)
{
	const char	*new_cursor;

	new_cursor = ft_strchrs((*cursor) + 1, charset);
	if (delim)
		new_cursor++;
	if (!new_cursor)
	{
		*res = ft_strndup(*cursor, ft_strlen(*cursor));
		if (!*res)
			return (COMMAND_ERROR);
		*cursor += ft_strlen(*cursor);
	}
	else
	{
		*res = ft_strndup(*cursor, new_cursor - *cursor);
		if (!*res)
			return (COMMAND_ERROR);
		*cursor = new_cursor;
	}
	return (COMMAND_SUCCESS);
}

t_command_status	get_next_heredoc_eof(const char **cursor, char **eof)
{
	t_command_status	status;
	t_list				*words;
	char				*word;

	words = NULL;
	while (**cursor)
	{
		if (ft_is_char_in_set(**cursor, "<>)&| "))
			break ;
		else if (**cursor == '\"')
			status = get_next_word_new(cursor, &word, "\"", true);
		else if (**cursor == '\'')
			status = get_next_word_new(cursor, &word, "\'", true);
		else
			status = get_next_word_new(cursor, &word, "\'\"<>)&| ", false);
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
	}
	*eof = ft_strsjoin_from_list(words);
	if (!eof)
		return (ft_lstclear(&words, free), COMMAND_ERROR);
	return (ft_lstclear(&words, free), COMMAND_SUCCESS);
}

t_command_status	get_next_expression(const char **ptr, char **res)
{
	t_command_status	status;
	t_list				*words;
	char				*word;
	const char			*cursor;
	bool				parenthesis;

	parenthesis = false;
	cursor = *ptr;
	words = NULL;
	while (*cursor)
	{
		if (!parenthesis && (!ft_strncmp(cursor, "||", 2) || !ft_strncmp(cursor,
					"&&", 2)))
			break ;
		if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"", true);
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else
		{
			if (*cursor == '(')
				parenthesis = true;
			else if (*cursor == ')')
				parenthesis = false;
			status = get_next_word_new(&cursor, &word, "\'\"()&|", false);
		}
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
	}
	*res = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	*ptr = cursor;
	return (COMMAND_SUCCESS);
}

t_command_status	get_next_command(const char **ptr, char **res)
{
	t_command_status	status;
	t_list				*words;
	char				*word;
	const char			*cursor;
	bool				parenthesis;

	parenthesis = false;
	cursor = *ptr;
	words = NULL;
	while (*cursor)
	{
		if (!parenthesis && *cursor == '|')
			break ;
		if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"", true);
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else
		{
			if (*cursor == '(')
				parenthesis = true;
			else if (*cursor == ')')
				parenthesis = false;
			status = get_next_word_new(&cursor, &word, "\'\"()|", false);
		}
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
	}
	*res = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	*ptr = cursor;
	return (COMMAND_SUCCESS);
}

t_command_status	get_next_parenthesis_expression(const char **ptr,
		char **res)
{
	t_command_status	status;
	t_list				*words;
	char				*word;
	const char			*cursor;

	cursor = *ptr;
	words = NULL;
	while (*cursor)
	{
		if (*cursor == ')')
		{
			cursor--;
			status = get_next_word_new(&(cursor), &word, ")", true);
		}
		else if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"", true);
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else
			status = get_next_word_new(&cursor, &word, "\'\")", false);
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
		if (word[ft_strlen(word) - 1] == ')')
			break ;
	}
	*res = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	*ptr = cursor;
	return (COMMAND_SUCCESS);
}

t_command_status	get_next_token(const char **ptr, char **res)
{
	t_command_status	status;
	t_list				*words;
	char				*word;
	const char			*cursor;

	cursor = *ptr;
	words = NULL;
	while (*cursor)
	{
		if (ft_is_char_in_set(*cursor, "<>&|() "))
			break ;
		else if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"", true);
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else
			status = get_next_word_new(&cursor, &word, "\'\"<>&|() ", false);
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
	}
	*res = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	return (*ptr = cursor, COMMAND_SUCCESS);
}

t_command_status	handle_word_interpretation(t_state *state, char **ptr)
{
	t_command_status	status;
	t_list				*words;
	char				*word;
	const char			*cursor;

	cursor = *ptr;
	words = NULL;
	while (*cursor)
	{
		if (*cursor == '$')
		{
			cursor++;
			status = get_var_value(state, &cursor, &word);
		}
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else
			status = get_next_word_new(&cursor, &word, "\'$", false);
		if (status)
			return (free(word), ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (free(word), ft_lstclear(&words, free), COMMAND_ERROR);
	}
	free(*ptr);
	*ptr = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	if (!*ptr)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

t_command_status	trim_quotes(char **str)
{
	char	*trimmed_str;

	if (!ft_is_char_in_set(**str, "\'\""))
		return (COMMAND_SUCCESS);
	trimmed_str = ft_strndup(&(*str)[1], ft_strlen(&(*str)[1]) - 1);
	if (!trimmed_str)
		return (COMMAND_ERROR);
	free(*str);
	*str = trimmed_str;
	return (COMMAND_SUCCESS);
}

t_command_status	suppr_quotes(char **ptr)
{
	t_command_status	status;
	t_list				*words;
	char				*word;
	const char			*cursor;

	cursor = *ptr;
	words = NULL;
	while (*cursor)
	{
		if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"", true);
		else
			status = get_next_word_new(&cursor, &word, "\'\"", false);
		if (status)
			return (ft_lstclear(&words, free), status);
		status = trim_quotes(&word);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
	}
	free(*ptr);
	*ptr = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	if (!*ptr)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}
