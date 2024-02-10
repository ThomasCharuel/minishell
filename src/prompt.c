/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:37:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/10 12:11:51 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*prompt_loop(t_state *state)
{
	char		*pwd;
	char		*prompt;
	char		*line;
	const char	*user;

	pwd = (char *)malloc(PATH_MAX * sizeof(char));
	if (!pwd)
		return (perror("minishell"), NULL);
	if (!getcwd(pwd, PATH_MAX))
		return (free(pwd), NULL);
	user = envp_get(state, "USER");
	if (!user)
		prompt = ft_strsjoin("$ ", NULL);
	else
		prompt = ft_strsjoin((char *)user, ":", pwd, "$ ", NULL);
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
	return (COMMAND_SIGINT);
}
