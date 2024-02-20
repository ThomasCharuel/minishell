/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:21:40 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 01:37:41 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
t_command_status	handle_word(t_state *state, const char **ptr, char **res,
		t_command_status parse_function(t_state *, const char **, t_list **))
{
	t_command_status	status;
	t_list				*words;

	words = NULL;
	status = parse_function(state, ptr, &words);
	if (status)
		return (ft_lstclear(&words, free), status);
	*res = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	if (!*res)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	get_next_word(const char **cursor, char **res,
		const char *charset, bool delim)
{
	const char	*new_cursor;

	new_cursor = ft_strchrs((*cursor) + 1, charset);
	if (!new_cursor)
	{
		*res = ft_strndup(*cursor, ft_strlen(*cursor));
		*cursor += ft_strlen(*cursor);
	}
	else
	{
		if (delim)
			new_cursor++;
		*res = ft_strndup(*cursor, new_cursor - *cursor);
		*cursor = new_cursor;
	}
	if (!*res)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	get_next_word_char(const char **cursor, char **res, char c,
		bool delim)
{
	char	charset[2];

	charset[0] = c;
	charset[1] = '\0';
	return (get_next_word(cursor, res, charset, delim));
}

t_command_status	get_next_heredoc_eof(t_state *state, const char **cursor,
		t_list **words)
{
	t_command_status	status;
	char				*word;

	(void)state;
	while (**cursor)
	{
		if (ft_is_char_in_set(**cursor, "<>)&| "))
			break ;
		else if (**cursor == '\"')
			status = get_next_word(cursor, &word, "\"", true);
		else if (**cursor == '\'')
			status = get_next_word(cursor, &word, "\'", true);
		else
			status = get_next_word(cursor, &word, "\'\"<>)&| ", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}

t_command_status	get_next_token(t_state *state, const char **ptr,
		t_list **words)
{
	t_command_status	status;
	char				*word;
	const char			*cursor;

	(void)state;
	cursor = *ptr;
	while (*cursor)
	{
		if (ft_is_char_in_set(*cursor, "<>&|() "))
			break ;
		else if (*cursor == '\"')
			status = get_next_word(&cursor, &word, "\"", true);
		else if (*cursor == '\'')
			status = get_next_word(&cursor, &word, "\'", true);
		else
			status = get_next_word(&cursor, &word, "\'\"<>&|() ", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (COMMAND_ERROR);
	}
	*ptr = cursor;
	return (COMMAND_SUCCESS);
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
			status = get_next_word(&cursor, &word, "\'", true);
		else
			status = get_next_word(&cursor, &word, "\'$", false);
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
			status = get_next_word(&cursor, &word, "\'", true);
		else if (*cursor == '\"')
			status = get_next_word(&cursor, &word, "\"", true);
		else
			status = get_next_word(&cursor, &word, "\'\"", false);
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
