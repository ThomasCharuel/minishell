/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dict.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:14:55 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 14:08:40 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dict	*ft_dict_init(void)
{
	t_dict	*dict;

	dict = (t_dict *)malloc(sizeof(t_dict));
	if (!dict)
		return (NULL);
	dict->list = NULL;
	return (dict);
}

char	*ft_dict_get(t_dict *dict, char *key)
{
	t_list	*node;
	t_item	*item;

	node = dict->list;
	while (node)
	{
		item = (t_item *)node->content;
		if (ft_strcmp(item->key, key))
			return (item->value);
		node = node->next;
	}
	return (NULL);
}

t_return_code	ft_dict_set(t_dict *dict, char *key, char *value)
{
	t_list	*node;
	t_item	*item;

	node = dict->list;
	while (node)
	{
		item = (t_item *)node->content;
		if (ft_strcmp(item->key, key))
			return (free(item->value), item->value = value, SUCCESS);
		node = node->next;
	}
	item = (t_item *)malloc(sizeof(t_item));
	node = ft_lstnew(item);
	if (!item)
		return (ERROR);
	node = ft_lstnew(item);
	if (!node)
		return (free(item), ERROR);
	item->key = key;
	item->value = value;
	ft_lstadd_back(&dict->list, node);
	return (SUCCESS);
}

void	ft_delete_item(t_item *item)
{
	free(item->key);
	free(item->value);
	free(item);
}

void	ft_dict_delete(t_dict *dict, char *key)
{
	t_list	*node;
	t_item	*item;

	node = dict->list;
	while (node)
	{
		item = (t_item *)node->content;
		if (ft_strcmp(item->key, key))
		{
			ft_lstdelone(node, ft_delete_item);
			return ;
		}
		node = node->next;
	}
}

void	ft_dict_destroy(t_dict **dict)
{
	ft_lstclear((*dict)->list, ft_delete_item);
	*dict = NULL;
}
