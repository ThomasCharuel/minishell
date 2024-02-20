/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 11:38:15 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_code;

int	main(int argc, char **argv, const char **envp)
{
	int		exit_code;
	t_state	*state;

	signal_init();
	state = state_init(argv[0], envp);
	if (!state)
		return (COMMAND_ERROR);
	if (argc == 2)
	{
		state->readline = ft_strdup(argv[1]);
		if (!state->readline)
			return (perror("minishell"), COMMAND_ERROR);
		exit_code = command_line_execute(state);
	}
	else
		exit_code = repl(state);
	state_cleanup(state);
	return (exit_code);
}
