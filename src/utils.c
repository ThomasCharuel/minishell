/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:59:02 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 10:22:50 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
void	ft_close_fd(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		close(fd);
}

// OK
void	print_error(const char *str, ...)
{
	va_list	ap;

	write(STDERR_FILENO, "minishell: ", 12);
	va_start(ap, str);
	while (str)
	{
		write(STDERR_FILENO, str, ft_strlen(str));
		str = va_arg(ap, char *);
	}
	va_end(ap);
	write(STDERR_FILENO, "\n", 1);
}

// OK
char	*get_working_directory(void)
{
	char	*pwd;

	pwd = ft_calloc(PATH_MAX, sizeof(char));
	if (!pwd)
		return (perror("minishell"), NULL);
	getcwd(pwd, PATH_MAX);
	return (pwd);
}

// OK
bool	is_whitespace_str(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (false);
		i++;
	}
	return (true);
}

// OK
char	**get_strs_from_list(t_list *list)
{
	char	**strs;
	size_t	i;

	i = ft_lst_len(list);
	strs = ft_calloc(i + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	i = 0;
	while (list)
	{
		strs[i++] = list->content;
		list = list->next;
	}
	return (strs);
}
