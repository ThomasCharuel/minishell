/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 12:42:06 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 13:40:22 by rdupeux          ###   ########.fr       */
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
	while (str[i] == ' ')
		i++;
	if (str[i])
		return (false);
	return (true);
}

size_t	clean_str_len(const char *str)
{
	size_t	len;

	len = 0;
	while (*str == ' ')
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		str++;
		len++;
	}
	while (*str == '0' && str[1] && str[1] != ' ')
		str++;
	while (*str && *str != ' ')
	{
		len++;
		str++;
	}
	return (len);
}

char	*clean_str(const char *str)
{
	char	*clean_str;
	size_t	i;

	clean_str = ft_calloc(clean_str_len(str) + 1, sizeof(char));
	if (!clean_str)
		return (perror("minishell"), NULL);
	i = 0;
	while (*str == ' ')
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		clean_str[i++] = *(str++);
	while (*str == '0' && str[1] && str[1] != ' ')
		str++;
	while (*str && *str != ' ')
		clean_str[i++] = *(str++);
	if (!ft_strcmp(clean_str, "-0"))
	{
		clean_str[0] = '0';
		clean_str[1] = '\0';
	}
	return (clean_str);
}

unsigned char	get_exit_code(const char *str)
{
	long	l_str;
	char	*cleaned_str;

	cleaned_str = clean_str(str);
	if (!cleaned_str)
		return (COMMAND_ERROR);
	if ((*cleaned_str == '-' && ft_strcmp(cleaned_str,
				"-9223372036854775808") > 0) || (*cleaned_str != '-'
			&& ft_strcmp(cleaned_str, "9223372036854775807") > 0))
		return (free(cleaned_str), COMMAND_PARSING_ERROR);
	l_str = ft_atol(str);
	free(cleaned_str);
	return (l_str);
}

t_command_status	minishell_exit(t_state *state, int argc, char **argv)
{
	t_command_status	status;

	(void)state;
	g_signal_code = SIGUSR1;
	if (argc == 1)
		status = COMMAND_SUCCESS;
	else if (argc == 2)
	{
		if (is_numeric_str(argv[1]))
			status = get_exit_code(argv[1]);
		else
			status = COMMAND_PARSING_ERROR;
	}
	else if (is_numeric_str(argv[1]))
		status = COMMAND_TOO_MANY_ARGUMENTS;
	else
		status = COMMAND_PARSING_ERROR;
	ft_printf("exit\n");
	if (status == COMMAND_TOO_MANY_ARGUMENTS)
		print_error("exit: ", "too many arguments", NULL);
	else if (status == COMMAND_PARSING_ERROR)
		print_error("exit: ", argv[1], ": numeric argument required", NULL);
	return (status);
}
