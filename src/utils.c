/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:59:02 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/16 14:22:24 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	ft_free_str(char **str)
{
	ft_free((void **)str);
}

void	ft_clean_double_list(void **list, void (*destroy)(void *))
{
	void	**temp;

	temp = list;
	if (list)
	{
		while (*temp)
			destroy(*temp++);
		free(list);
	}
}

void	ft_close_fd(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		close(fd);
}

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

char	*get_working_directory(void)
{
	char	*pwd;

	pwd = ft_calloc(PATH_MAX, sizeof(char));
	if (!pwd)
		return (perror("minishell"), NULL);
	if (!getcwd(pwd, PATH_MAX))
		return (perror("minishell"), free(pwd), NULL);
	return (pwd);
}
