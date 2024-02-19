/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:37:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/19 16:13:58 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_whitespace_line(const char *line)
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

static t_command_status	prompt_loop(t_state *state, char **ptr)
{
	char	*pwd;
	char	*prompt;

	pwd = get_working_directory();
	if (!pwd)
		return (COMMAND_ERROR);
	prompt = ft_strsjoin(envp_get(state, "USER"), ":", pwd, "$ ", NULL);
	if (!prompt)
		return (perror("minishell"), free(pwd), COMMAND_ERROR);
	free(pwd);
	*ptr = readline(prompt);
	free(prompt);
	return (COMMAND_SUCCESS);
}

t_command_status	repl(t_state *state)
{
	char				*line;
	t_command_status	status;

	status = COMMAND_SUCCESS;
	while (g_signal_code != SIGUSR1 && status != COMMAND_ERROR)
	{
		status = prompt_loop(state, &line);
		if (status)
			return (status);
		if (!line)
			break ;
		if (g_signal_code == SIGINT)
		{
			state->last_exit_code = COMMAND_SIGINT;
			g_signal_code = 0;
		}
		if (g_signal_code == SIGQUIT)
		{
			state->last_exit_code = COMMAND_SIGQUIT;
			g_signal_code = 0;
		}
		if (*line)
		{
			add_history(line);
			if (!is_whitespace_line(line))
				status = command_line_execute(state, line);
		}
		free(line);
	}
	write(STDOUT_FILENO, "exit\n", 6);
	return (state->last_exit_code);
}
