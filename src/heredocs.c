/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:28:47 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 14:11:45 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
t_command_status	handle_heredocs(t_state *state, const char **ptr,
		t_list **words)
{
	t_command_status	status;
	char				*word;

	while (*ptr && **ptr)
	{
		if (**ptr == '\"')
			status = get_next_word(ptr, &word, "\"", true);
		else if (**ptr == '\'')
			status = get_next_word(ptr, &word, "\'", true);
		else if (!ft_strncmp(*ptr, "<<", 2))
			status = handle_heredoc(state, ptr, &word);
		else
			status = get_next_word(ptr, &word, "\'\"<", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (ft_free_str(&word), COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}

t_command_status	write_heredoc_decorator(t_heredoc *heredoc)
{
	int					fd_stdin;
	t_command_status	status;

	signal_init_heredoc();
	fd_stdin = dup(STDIN_FILENO);
	status = write_heredoc(heredoc);
	if (status == COMMAND_SIGINT)
	{
		g_signal_code = 0;
		dup2(fd_stdin, STDIN_FILENO);
		unlink(heredoc->file);
	}
	close(fd_stdin);
	signal_init();
	return (status);
}
