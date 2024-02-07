/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:59:02 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/07 13:42:08 by tcharuel         ###   ########.fr       */
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

char	*ft_strsjoin_from_list(t_list *list)
{
	char	**strs;
	char	*str;
	size_t	len;

	len = ft_lst_len(list);
	strs = calloc(len + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	len = 0;
	while (list)
	{
		strs[len++] = list->content;
		list = list->next;
	}
	str = ft_strsjoin_array((const char **)strs);
	free(strs);
	return (str);
}

void	ft_clean_double_list(void **list, void (*destroy)(void *))
{
	void	**temp;

	temp = list;
	if (list)
	{
		while (*temp)
		{
			destroy(*temp);
			temp++;
		}
		free(list);
	}
}

void	ft_close_fd(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		close(fd);
}
