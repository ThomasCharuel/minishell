/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:59:02 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/10 00:32:57 by tcharuel         ###   ########.fr       */
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
