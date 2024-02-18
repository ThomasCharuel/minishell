/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:13:26 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/18 17:39:18 by tcharuel         ###   ########.fr       */
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
	if (!access(file, F_OK))
		return (get_heredoc_file_name(should_be_interpreted));
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
	status = get_func_decorator(cursor, &eof, &get_next_heredoc_eof);
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
