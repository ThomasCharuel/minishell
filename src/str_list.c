/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:34:20 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/15 13:21:16 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_str_list(t_list *word_list)
{
	while (word_list)
	{
		ft_printf("- %s\n", word_list->content);
		word_list = word_list->next;
	}
}

t_return_status	str_list_append(t_list **word_list, const char *str)
{
	if (!str || !ft_strlen(str))
		return (SUCCESS);
	if (!ft_append(word_list, (void *)str))
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
