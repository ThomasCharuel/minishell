/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:37:41 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/09 21:26:05 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_node(t_node *node)
{
	if (node->type == COMMAND)
		printf("command node: %s\n", ((t_command *)node->content)->command_str);
	if (node->type == PIPE)
		printf("PIPE\n");
	if (node->type == AND)
		printf("AND\n");
	if (node->type == OR)
		printf("OR\n");
}

t_command_status	ast_execute(t_state *state, t_node *node)
{
	t_command_status	status;

	if (node->type == AND)
	{
		status = ast_execute(state, node->left);
		if (status)
			return (status);
		return (ast_execute(state, node->right));
	}
	else if (node->type == OR)
	{
		status = ast_execute(state, node->left);
		if (!status)
			return (status);
		return (ast_execute(state, node->right));
	}
	else if (node->type == PIPE)
	{
		// TODO
		status = ast_execute(state, node->left);
		if (status)
			return (status);
		return (ast_execute(state, node->right));
	}
	status = command_parse(state, node->content);
	if (status)
		return (status);
	if (!command_exec(state, node->content, STDIN_FILENO))
		return (COMMAND_ERROR);
	while (wait(NULL) != -1)
		// Mettre pid dans une liste chaine et get le derniere status
		continue ;
	return (COMMAND_SUCCESS);
}
