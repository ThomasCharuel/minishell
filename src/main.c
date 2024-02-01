/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/01 14:43:54 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ctrl + C
void	sigint_handler(int signum)
{
	(void)signum;
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Ctrl + D
void	sigterm_handler(int signum)
{
	(void)signum;
	exit(EXIT_SUCCESS);
}

// Ctrl + backslash
void	sigquit_handler(int signum)
{
	(void)signum;
}

int	main(int argc, char **argv)
{
	char	*line;

	(void)argc;
	(void)argv;
	signal(SIGTERM, &sigterm_handler);
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, &sigquit_handler);
	while (true)
	{
		line = prompt_loop();
		if (!is_whitespace_line(line))
		{
			add_history(line);
			ft_printf("%s\n", line);
		}
		free(line);
	}
	ft_printf("\nexit\n");
	return (0);
}
