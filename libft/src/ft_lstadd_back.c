/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:19:29 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 17:50:01 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Adds the node ’new’ at the end of the list.
 *
 * @param lst: The address of a pointer to the first link of
 * a list.
 * @param new: The address of a pointer to the node to be
 * added to the list.
 */
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*node;

	if (!*lst)
		*lst = new;
	else
	{
		node = *lst;
		while (node->next)
		{
			node = node->next;
		}
		node->next = new;
	}
}
