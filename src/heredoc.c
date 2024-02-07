/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:13:26 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/07 18:32:36 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	get_next_heredoc_word(void)
{
	const char	*cursor;
	const char	*new_cursor;

	while (*cursor)
	{
		new_cursor = ft_strchrs(new_cursor, "\"\'<");
		if (!new_cursor)
		{
		}
	}
}

t_command_status	handle_heredocs(t_command *command)
{
	t_list *words;
	char *cursor;
	char *word;
	words = NULL;

	cursor = command->command_str;

	while (*cursor)
	{
		status = get_next_heredoc_word(&cursor, &word);
		if (status)
			return (status);
		if (!word)
			break ;
		status = ft_printf("WORD: %s\n", word);
	}
	return (COMMAND_SUCCESS);
}