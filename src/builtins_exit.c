/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 12:42:06 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/22 13:35:35 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_numeric_str(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] && str[i] != ' ')
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	while (str[i++] == ' ')
		;
	if (str[i])
		return (false);
	return (true);
}

unsigned char	get_exit_code(const char *str)
{
	long		l_str;
	const char	*end_str;
	char		*match;

	l_str = ft_atol(str);
	match = ft_ltoa(l_str, "0123456789");
	if (!match)
		return (COMMAND_ERROR);
	str = ft_strchrs(str, "-0123456789");
	if (!str)
		return (free(match), print_error("syntax error", NULL),
			COMMAND_PARSING_ERROR);
	end_str = ft_strchr(str, ' ');
	if ((end_str && ft_strncmp(match, str, end_str - str)) || (!end_str
			&& ft_strcmp(match, str)))
		return (free(match), print_error("syntax error", NULL),
			COMMAND_PARSING_ERROR);
	free(match);
	return (l_str);
}

t_command_status	minishell_exit(t_state *state, int argc, char **argv)
{
	t_command_status	status;

	(void)state;
	status = COMMAND_SUCCESS;
	g_signal_code = SIGUSR1;
	if (argc == 1)
		return (status);
	else if (argc > 2)
		status = COMMAND_TOO_MANY_ARGUMENTS;
	if (argc > 2 && !is_numeric_str(argv[1]))
		status = COMMAND_PARSING_ERROR;
	else
		status = get_exit_code(argv[1]);
	ft_printf("exit\n");
	if (status == COMMAND_TOO_MANY_ARGUMENTS)
		print_error("exit: ", "too many arguments", NULL);
	else if (status == COMMAND_PARSING_ERROR)
		print_error("exit: ", argv[1], ": numeric argument required", NULL);
	return (status);
}
