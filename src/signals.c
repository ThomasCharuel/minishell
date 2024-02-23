/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:50:39 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 14:22:25 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sig_handler_heredoc(int signum)
{
	g_signal_code = signum;
	ft_printf("^C");
	close(STDIN_FILENO);
}

static void	sig_handler(int signum)
{
	g_signal_code = signum;
	if (wait(NULL) != -1)
	{
		if (signum == SIGQUIT)
			ft_printf("Quit (core dumped)");
		ft_printf("\n");
	}
	else if (signum == SIGINT)
	{
		ft_printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
		g_signal_code = 0;
}

void	signal_init(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = &sig_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	signal_init_heredoc(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = &sig_handler_heredoc;
	sigaction(SIGINT, &sa, NULL);
}
