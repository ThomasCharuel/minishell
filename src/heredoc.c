/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:13:26 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/12 12:00:32 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_heredoc_file_name(bool should_be_interpreted)
{
	char	*file;
	char	*uid;

	uid = ft_rand_uuid();
	if (!uid)
		return (NULL);
	if (should_be_interpreted)
		file = ft_strsjoin("/tmp/.minishell-hi-", uid, NULL);
	else
		file = ft_strsjoin("/tmp/.minishell-h-", uid, NULL);
	free(uid);
	return (file);
}

t_heredoc	*heredoc_create(char *eof)
{
	t_heredoc	*heredoc;

	heredoc = malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	if (ft_strchrs(eof, "\'\""))
		heredoc->file = get_heredoc_file_name(false);
	else
		heredoc->file = get_heredoc_file_name(true);
	if (!heredoc->file)
		return (free(heredoc->file), free(heredoc), NULL);
	ft_printf("Heredoc name: %s\n", heredoc->file);
	heredoc->eof = eof;
	return (heredoc);
}

void	heredoc_destroy(void *ptr)
{
	t_heredoc	*heredoc;

	heredoc = ptr;
	if (heredoc)
	{
		unlink(heredoc->file);
		free(heredoc->file);
		free(heredoc->eof);
		free(heredoc);
	}
}

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

t_command_status	heredoc_get_eof(const char **cursor, char **eof)
{
	t_command_status	status;
	t_list				*words;
	char				*word;

	words = NULL;
	while (**cursor)
	{
		if (ft_is_char_in_set(**cursor, "<> "))
			break ;
		else if (**cursor == '\"')
			status = get_next_word_new(cursor, &word, "\"", true);
		else if (**cursor == '\'')
			status = get_next_word_new(cursor, &word, "\'", true);
		else
			status = get_next_word_new(cursor, &word, "\'\"<> ", false);
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

t_return_status	write_heredoc(t_heredoc *heredoc)
{
	char	*line;
	int		fd;

	fd = open(heredoc->file, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd < 0)
		return (perror(heredoc->file), ERROR);
	line = readline(">");
	while (line && ft_strcmp(line, heredoc->eof))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
	}
	if (line)
		free(line);
	close(fd);
	return (SUCCESS);
}

t_command_status	handle_heredoc(t_state *state, const char **cursor,
		char **res)
{
	t_command_status	status;
	char				*eof;
	t_heredoc			*heredoc;

	*cursor += 2;
	while (**cursor == ' ')
		(*cursor)++;
	status = heredoc_get_eof(cursor, &eof);
	if (status)
		return (ft_free_str(&eof), status);
	if (!ft_strlen(eof))
		return (ft_free_str(&eof), COMMAND_PARSING_ERROR);
	heredoc = heredoc_create(eof);
	if (!heredoc)
		return (ft_free_str(&eof), COMMAND_ERROR);
	if (!ft_append(&state->heredocs, heredoc))
		return (heredoc_destroy(heredoc), COMMAND_ERROR);
	*res = ft_strsjoin("<", heredoc->file, NULL);
	if (!write_heredoc(heredoc))
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

t_command_status	handle_heredocs(t_state *state, const char *line)
{
	t_command_status	status;
	const char			*cursor;
	t_list				*words;
	char				*word;

	words = NULL;
	cursor = line;
	while (*cursor)
	{
		if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"", true);
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else if (!ft_strncmp(cursor, "<<", 2))
			status = handle_heredoc(state, &cursor, &word);
		else
			status = get_next_word_new(&cursor, &word, "\'\"<", false);
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
	}
	state->line = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	if (!state->line)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}
