/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 20:49:59 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/05 19:45:46 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*command_create(const char *command_str)
{
	t_command	*command;

	command = (t_command *)malloc(sizeof(t_command));
	if (!command)
		return (perror("minishell"), NULL);
	command->command_str = (char *)command_str;
	command->argv = NULL;
	command->redirections = NULL;
	command->in_fd = STDIN_FILENO;
	command->out_fd = STDOUT_FILENO;
	return (command);
}

void	command_destroy(void *ptr)
{
	t_command	*command;

	command = ptr;
	if (command)
	{
		free(command->command_str);
		ft_lstclear(&command->argv, free);
		ft_lstclear(&command->redirections, redirection_destroy);
		free(command);
	}
}

t_command_status	set_command_command(t_state *state, t_command *command)
{
	t_command_status	status;

	if (!command->argv)
		return (COMMAND_NOT_FOUND);
	if (ft_strchr((char *)command->argv->content, '/'))
		status = handle_path_command(command);
	else
		status = handle_command(state, command);
	return (COMMAND_SUCCESS);
}

bool	is_redirection_char(const char c)
{
	if (c == '<' || c == '>')
		return (true);
	return (false);
}

t_redirection_type	get_redirection_type(const char *word)
{
	if (word[0] == '>')
	{
		if (word[1] == '>')
			return (APPEND);
		return (WRITE);
	}
	return (READ);
}

t_command_status	command_parse(t_state *state, t_command *command)
{
	size_t			i;
	char			**words;
	t_redirection	*redirection;

	words = ft_split(command->command_str, ' ');
	if (!words)
		return (COMMAND_ERROR);
	i = 0;
	while (words[i])
	{
		if (is_redirection_char(words[i][0]))
		{
			if (!words[i][1])
			{
				i++;
				if (!words[i] || is_redirection_char(words[i][0]))
					return (COMMAND_PARSING_ERROR);
				redirection = redirection_create(words[i],
						get_redirection_type(words[i - 1]));
			}
			else if (!is_redirection_char(words[i][1]))
			{
				redirection = redirection_create(&words[i][1],
						get_redirection_type(words[i]));
			}
			else if (words[i][0] == words[i][1])
			{
				// Concat ou heredoc
			}
			else
				return (ft_clean_double_list((void **)words, free),
					COMMAND_PARSING_ERROR);
			if (!redirection || !lst_append_content(&(command->redirections),
					redirection))
				return (ft_clean_double_list((void **)words, free),
					COMMAND_ERROR);
		}
		else if (!lst_append_content(&(command->argv), words[i]))
			return (ft_clean_double_list((void **)words, free),
				COMMAND_PARSING_ERROR);
		i++;
	}
	ft_clean_double_list((void **)words, free);
	return (set_command_command(state, command));
}
