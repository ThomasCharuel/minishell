/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:50:39 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 11:57:28 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ctrl + C
void	sigint_handler(int signal_code)
{
	g_signal_code = signal_code;
	ft_printf("^C\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Ctrl + D
void	sigterm_handler(int signal_code)
{
	g_signal_code = signal_code;
	ft_printf("exit\n");
}

// Ctrl + backslash
void	sigquit_handler(int signal_code)
{
	g_signal_code = signal_code;
	ft_printf("Yo\n");
}

void	signal_init(void)
{
	rl_catch_signals = 0;
	signal(SIGTERM, &sigterm_handler);
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, &sigquit_handler);
}
