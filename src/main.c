/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 19:26:07 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_code;

int	main(int argc, char **argv, const char **envp)
{
	char	*line;
	t_state	*state;

	(void)argc;
	(void)argv;
	signal_init();
	state = state_init(envp);
	if (!state)
		return (1);
	while (g_signal_code != SIGTERM)
	{
		line = prompt_loop();
		if (!line)
			break ;
		if (!is_whitespace_line(line))
		{
			add_history(line);
			if (!ft_exec(state, line))
			{
				free(line);
				break ;
			}
		}
		free(line);
	}
	state_cleanup(state);
	return (0);
}
