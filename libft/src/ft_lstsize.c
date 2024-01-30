/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:31:27 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 13:34:23 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Counts the number of nodes in a list.
 *
 * @param lst: The address of a pointer to the first link of
 * a list.
 * 
 * @return The length of the list.
 */
int	ft_lstsize(t_list *lst)
{
	size_t	len;
	t_list	*node;

	len = 0;
	node = lst;
	while (node)
	{
		node = node->next;
		len++;
	}
	return (len);
}
