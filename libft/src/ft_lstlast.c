/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:35:16 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 13:38:04 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Returns the last node of the list.
 *
 * @param lst: The address of a pointer to the first link of
 * a list.
 * 
 * @return Last node of the list.
 */
t_list	*ft_lstlast(t_list *lst)
{
	t_list	*node;

	if (!lst)
		return (NULL);
	node = lst;
	while (node->next)
		node = node->next;
	return (node);
}
