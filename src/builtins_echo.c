/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:10:57 by rdupeux           #+#    #+#             */
/*   Updated: 2024/02/22 11:21:35 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static bool	is_n_option(char *str)
{
	size_t	i;

	if (*str != '-')
		return (false);
	i = 1;
	while (str[i] == 'n')
		i++;
	if (!str[i])
		return (true);
	return (false);
}

t_command_status	minishell_echo(t_state *state, int argc, char **argv)
{
	int		i;
	bool	should_print_new_line;

	(void)state;
	should_print_new_line = true;
	i = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		should_print_new_line = false;
		i++;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i++]);
		if (i < argc)
			ft_printf(" ");
	}
	if (should_print_new_line)
		ft_printf("\n");
	return (COMMAND_SUCCESS);
}
