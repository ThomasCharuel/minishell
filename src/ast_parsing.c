/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:03:54 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/19 17:12:23 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command_status	build_ast(t_state *state, const char **cursor,
		t_node *left_child)
{
	t_command_status	status;
	t_node				*node;
	char				*word;

	if (!left_child)
	{
		status = get_func_decorator(cursor, &word, &get_next_expression);
		(void)status; // Handle status
		status = command_generation_handling((const char **)&word, &node);
		(void)status; // Handle status
	}
	else
	{
		if (!ft_strncmp(*cursor, "||", 2))
			node = node_create(OR, NULL);
		else if (!ft_strncmp(*cursor, "&&", 2))
			node = node_create(AND, NULL);
		else
			return (COMMAND_PARSING_ERROR);
		*cursor += 2;
		status = get_func_decorator(cursor, &word, &get_next_expression);
		(void)status; // Handle status
		status = command_generation_handling((const char **)&word,
				&node->right);
		node->right->daddy = node;
		(void)status; // Handle status
		left_child->daddy = node;
		node->left = left_child;
	}
	state->ast = node;
	free(word);
	if (**cursor)
		return (build_ast(state, cursor, node));
	return (COMMAND_SUCCESS);
}

t_command_status	ast_generate(t_state *state)
{
	const char	*command_line;

	command_line = state->line;
	return (build_ast(state, &command_line, NULL));
}
