/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:37:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/16 20:22:10 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*prompt_loop(t_state *state)
{
	char	*pwd;
	char	*prompt;
	char	*line;

	pwd = get_working_directory();
	if (!pwd)
		return (NULL);
	prompt = ft_strsjoin(envp_get(state, "USER"), ":", pwd, "$ ", NULL);
	if (!prompt)
		return (perror("minishell"), free(pwd), NULL);
	free(pwd);
	line = readline(prompt);
	free(prompt);
	return (line);
}

bool	is_whitespace_line(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (false);
		i++;
	}
	return (true);
}
t_command_status	repl(t_state *state)
{
	char				*line;
	t_command_status	status;

	while (g_signal_code != SIGTERM)
	{
		line = prompt_loop(state);
		if (!line)
			return (COMMAND_ERROR);
		if (*line)
			add_history(line);
		if (is_whitespace_line(line))
			continue ;
		status = line_exec(state, line);
		free(line);
		if (status == COMMAND_ERROR)
			return (status);
	}
	write(STDOUT_FILENO, "exit\n", 6);
	return (state->last_exit_code);
}
