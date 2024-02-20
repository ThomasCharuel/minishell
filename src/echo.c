/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:10:57 by rdupeux           #+#    #+#             */
/*   Updated: 2024/02/20 13:51:40 by rdupeux          ###   ########.fr       */
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
	if (argc > 1 && !ft_strcmp("-n", argv[i]))
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
