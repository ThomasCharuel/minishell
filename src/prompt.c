/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:37:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 14:10:20 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static t_command_status	prompt_loop(t_state *state)
{
	char	*pwd;
	char	*prompt;

	pwd = get_working_directory();
	if (!pwd)
		return (COMMAND_ERROR);
	prompt = ft_strsjoin(envp_get(state, "USER"), ":", pwd, "$ ", NULL);
	free(pwd);
	if (!prompt)
		return (perror("minishell"), COMMAND_ERROR);
	state->readline = readline(prompt);
	free(prompt);
	return (COMMAND_SUCCESS);
}

static bool	catch_signals(t_state *state)
{
	if (g_signal_code == SIGINT)
	{
		state->last_exit_code = COMMAND_SIGINT;
		g_signal_code = 0;
		return (true);
	}
	if (g_signal_code == SIGQUIT)
	{
		state->last_exit_code = COMMAND_SIGQUIT;
		g_signal_code = 0;
		return (true);
	}
	return (false);
}

t_command_status	repl(t_state *state)
{
	t_command_status	status;

	status = COMMAND_SUCCESS;
	while (g_signal_code != SIGUSR1 && status != COMMAND_ERROR)
	{
		status = prompt_loop(state);
		if (status)
			return (status);
		if (!state->readline)
			break ;
		catch_signals(state);
		if (*state->readline)
		{
			add_history(state->readline);
			if (!is_whitespace_str(state->readline))
				status = command_line_execute(state);
		}
	}
	if (g_signal_code != SIGUSR1)
		ft_printf("exit\n");
	return (state->last_exit_code);
}
