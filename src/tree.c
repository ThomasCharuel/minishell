/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:56:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 14:53:12 by rdupeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
t_node	*node_create(t_state *state, t_node_type type, void *content)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->content = content;
	node->daddy = NULL;
	node->left = NULL;
	node->right = NULL;
	node->read_fd = STDIN_FILENO;
	node->write_fd = STDOUT_FILENO;
	node->state = state;
	return (node);
}

// OK
void	node_destroy(t_node **node)
{
	if (*node)
	{
		if ((*node)->left)
			node_destroy(&(*node)->left);
		if ((*node)->right)
			node_destroy(&(*node)->right);
		if ((*node)->type == PIPE)
			free((*node)->content);
		else if ((*node)->type == COMMAND)
			command_destroy((*node)->content);
		free(*node);
		*node = NULL;
	}
}

// OK
void	tree_dfs(t_state *state, t_node *node, void (*f)(t_state *, t_node *))
{
	if (!node)
		return ;
	f(state, node);
	if (node->left)
		tree_dfs(state, node->left, f);
	if (node->right)
		tree_dfs(state, node->right, f);
}
