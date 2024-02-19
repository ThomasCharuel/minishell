/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:28:47 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/19 17:12:43 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	handle_heredocs(t_state *state, const char *str,
		t_list **words)
{
	t_command_status	status;
	char				*word;

	word = NULL;
	while (*str)
	{
		if (*str == '\"')
			status = get_next_word_new(&str, &word, "\"", true);
		else if (*str == '\'')
			status = get_next_word_new(&str, &word, "\'", true);
		else if (!ft_strncmp(str, "<<", 2))
			status = handle_heredoc(state, &str, &word);
		else
			status = get_next_word_new(&str, &word, "\'\"<", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (ft_free_str(&word), COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}
