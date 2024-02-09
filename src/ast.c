/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:37:41 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/09 15:23:06 by tcharuel         ###   ########.fr       */
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
