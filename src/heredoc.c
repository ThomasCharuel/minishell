/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:13:26 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/08 17:16:36 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pid(void)
{
	char	*res;
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid < 0)
		return (NULL);
	if (!child_pid)
		exit(0);
	res = ft_itoa(child_pid);
	return (res);
}

char	*get_heredoc_file_name(void)
{
	static size_t	id = 0;
	char			*file;
	char			*id_str;
	char			*pid_str;

	pid_str = get_pid();
	id_str = ft_lutoa(id, "0123456789abcdef");
	if (!id_str)
		return (free(pid_str), NULL);
	file = ft_strsjoin("/tmp/minishell-", pid_str, "-", id_str, NULL);
	free(pid_str);
	free(id_str);
	return (file);
}

t_heredoc	*heredoc_create(const char *eof)
{
	t_heredoc	*heredoc;

	heredoc = malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	if (ft_strchrs(eof, "\'\""))
		heredoc->should_be_interpreted = false;
	else
		heredoc->should_be_interpreted = true;
	heredoc->file = get_heredoc_file_name();
	if (!heredoc->file)
		return (free(heredoc), NULL);
	heredoc->fd = open(heredoc->file, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR);
	ft_printf("FD: %s\n", heredoc->file);
	if (heredoc->fd < 0)
		return (free(heredoc), NULL);
	unlink(heredoc->file);
	return (heredoc);
}

void	heredoc_destroy(void *ptr)
{
	t_heredoc	*heredoc;

	heredoc = ptr;
	if (heredoc)
	{
		close(heredoc->fd);
		free(heredoc->file);
		free(heredoc);
	}
}

t_command_status	get_next_word_new(const char **cursor, char **res,
		const char *charset)
{
	const char	*new_cursor;

	new_cursor = ft_strchrs((*cursor) + 1, charset);
	if (!new_cursor)
	{
		*res = strndup(*cursor, ft_strlen(*cursor));
		if (!*res)
			return (COMMAND_ERROR);
		*cursor += ft_strlen(*cursor);
	}
	else
	{
		*res = strndup(*cursor, new_cursor - *cursor);
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
			status = get_next_word_new(cursor, &word, "\"");
		else if (**cursor == '\'')
			status = get_next_word_new(cursor, &word, "\'");
		else
			status = get_next_word_new(cursor, &word, "\'\"<> ");
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
		return (free(eof), COMMAND_ERROR);
	if (!ft_append(&state->heredocs, heredoc))
		return (free(eof), heredoc_destroy(heredoc), COMMAND_ERROR);
	free(eof);
	*res = ft_strsjoin("< ", heredoc->file, NULL);
	return (COMMAND_SUCCESS);
}

t_command_status	command_handle_heredocs(t_state *state, t_command *command)
{
	t_command_status	status;
	const char			*cursor;
	t_list				*words;
	char				*word;

	words = NULL;
	cursor = command->command_str;
	while (*cursor)
	{
		if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"");
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'");
		else if (!ft_strncmp(cursor, "<<", 2))
			status = handle_heredoc(state, &cursor, &word);
		else
			status = get_next_word_new(&cursor, &word, "\'\"<");
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
		ft_printf("Heredoc Word is: %s\n", word);
	}
	ft_lstclear(&words, free);
	return (COMMAND_SUCCESS);
}