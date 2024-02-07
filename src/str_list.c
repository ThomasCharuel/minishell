/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:34:20 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/07 20:36:11 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_return_status	str_list_append(t_list **word_list, char *str)
{
	if (!str || !ft_strlen(str))
		return (SUCCESS);
	if (!ft_append(word_list, str))
		return (ERROR);
	return (SUCCESS);
}

char	*ft_strsjoin_from_list(t_list *list)
{
	const char	**strs;
	char		*str;
	size_t		len;

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
	str = ft_strsjoin_array(strs);
	free(strs);
	return (str);
}
