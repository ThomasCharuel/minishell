/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/01 11:55:58 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ctrl + C
void	sigInt(int code)
{
	(void)code;
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Ctrl + D
void	sigTerm(int code)
{
	(void)code;
	ft_printf("\nexit\n");
	exit(EXIT_SUCCESS);
}

// Ctrl + backslash
void	sigQuit(int code)
{
	(void)code;
	printf("yo\n");
}

int	main(int argc, char **argv)
{
	char	*line;

	(void)argc;
	(void)argv;
	signal(SIGTERM, &sigTerm);
	signal(SIGINT, &sigInt);
	signal(SIGQUIT, &sigQuit);
	line = prompt_loop();
	while (line)
	{
		if (!is_whitespace_line(line))
		{
			add_history(line);
			ft_printf("%s\n", line);
		}
		free(line);
		line = prompt_loop();
	}
	return (0);
}
