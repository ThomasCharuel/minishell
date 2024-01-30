/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 14:01:45 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 14:19:07 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Iterates the list ’lst’ and applies the function
 * ’f’ on the content of each node. Creates a new
 * list resulting of the successive applications of
 * the function ’f’. The ’del’ function is used to
 * delete the content of a node if needed.
 *
 * @param lst: The address of a pointer to a node.
 * @param f: The address of the function used to iterate on the list.
 * @param del: The address of the function used to delete
 * the content of a node if needed.
 * 
 * @return The new list.
 * @retval NULL if the allocation fails.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res_lst;
	t_list	*new_node;

	if (!lst)
		return (NULL);
	res_lst = NULL;
	while (lst)
	{
		new_node = ft_lstnew((*f)(lst->content));
		if (!new_node)
		{
			ft_lstclear(&res_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&res_lst, new_node);
		lst = lst->next;
	}
	return (res_lst);
}
