/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:50:39 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 16:20:03 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signum, siginfo_t *siginfo, void *context)
{
	(void)context;
	(void)siginfo;
	if (signum == SIGINT)
	{
		ft_printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGTERM)
	{
		ft_printf("exit\n");
	}
	else if (signum == SIGQUIT)
	{
		ft_printf("Yo\n");
	}
	g_signal_code = signum;
}

void	signal_init(void)
{
	struct sigaction	psa;

	rl_catch_signals = 0;
	psa.sa_sigaction = &sig_handler;
	sigaction(SIGINT, &psa, NULL);
	sigaction(SIGTERM, &psa, NULL);
	sigaction(SIGQUIT, &psa, NULL);
}
