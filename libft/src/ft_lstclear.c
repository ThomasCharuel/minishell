/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:48:52 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 15:25:22 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Deletes and frees the given node and every
 * successor of that node, using the function ’del’
 * and free(3).
 * Finally, the pointer to the list must be set to
 * NULL.
 *
 * @param lst: The address of a pointer to a node.
 * @param del: The address of the function used to delete
 * the content.
 */
void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	if (!lst || !*lst || !del)
		return ;
	if ((*lst)->next)
		ft_lstclear(&(*lst)->next, del);
	(*del)((*lst)->content);
	free(*lst);
	*lst = NULL;
}
