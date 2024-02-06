/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/06 10:15:59 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_code;

int	main(int argc, char **argv, const char **envp)
{
	int		exit_code;
	char	*line;
	t_state	*state;

	(void)argc;
	(void)argv;
	signal_init();
	state = state_init(envp);
	if (!state)
		return (EXIT_FAILURE);
	while (g_signal_code != SIGTERM)
	{
		line = prompt_loop();
		if (!line)
			break ;
		if (is_whitespace_line(line))
			continue ;
		add_history(line);
		state->last_exit_code = line_exec(state, line);
		if (state->last_exit_code == COMMAND_ERROR)
		{
			free(line);
			break ;
		}
		free(line);
	}
	ft_printf("exit\n");
	exit_code = state->last_exit_code;
	state_cleanup(state);
	return (exit_code);
}
