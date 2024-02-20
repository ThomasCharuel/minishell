/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:10:57 by rdupeux           #+#    #+#             */
/*   Updated: 2024/02/20 19:40:40 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_status	minishell_echo(t_state *state, int argc, char **argv)
{
	int		i;
	bool	should_print_new_line;

	(void)state;
	should_print_new_line = true;
	i = 1;
	while (argv[i] && !ft_strcmp("-n", argv[i]))
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
