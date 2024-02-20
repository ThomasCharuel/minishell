/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:22:40 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 10:28:37 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
void	ft_free(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

// OK
void	ft_free_str(char **str)
{
	ft_free((void **)str);
}

// OK
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

// OK
t_return_status	str_list_append(t_list **word_list, const char *str)
{
	if (!str)
		return (SUCCESS);
	if (!ft_append(word_list, (void *)str))
		return (ERROR);
	return (SUCCESS);
}

// OK
char	*ft_strsjoin_from_list(t_list *list)
{
	const char	**strs;
	char		*str;
	size_t		len;

	len = ft_lst_len(list);
	strs = ft_calloc(len + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	len = 0;
	while (list)
	{
		strs[len++] = list->content;
		list = list->next;
	}
	str = ft_strsjoin_array(strs);
	free(strs);
	return (str);
}
