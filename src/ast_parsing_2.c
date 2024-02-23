/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parsing_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 19:36:38 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 12:00:54 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command_status	get_next_subcommand_pipe(t_state *state,
		const char **ptr, t_list **words)
{
	t_command_status	status;
	char				*word;
	int					parenthesis_count;

	(void)state;
	parenthesis_count = 0;
	while (*ptr && **ptr)
	{
		if (!parenthesis_count && **ptr == '|')
			break ;
		if (**ptr == '(')
			parenthesis_count++;
		else if (**ptr == ')')
			parenthesis_count--;
		if (ft_is_char_in_set(**ptr, "\'\""))
			status = get_next_word_char(ptr, &word, **ptr, true);
		else
			status = get_next_word(ptr, &word, "\'\"()|", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (ft_free_str(&word), COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}

static t_command_status	ast_generate_lower_nodes_pipes(t_state *state,
		const char **ptr, t_node **daddy, t_command *command)
{
	t_command_status	status;

	*daddy = node_create(state, PIPE, NULL);
	if (!*daddy)
		return (command_destroy(command), COMMAND_ERROR);
	(*daddy)->left = node_create(state, COMMAND, command);
	if (!(*daddy)->left)
		return (node_destroy(daddy), command_destroy(command), COMMAND_ERROR);
	(*daddy)->left->daddy = *daddy;
	(*ptr)++;
	status = ast_generate_lower_nodes(state, ptr, &(*daddy)->right);
	if (status)
		return (status);
	(*daddy)->right->daddy = *daddy;
	return (COMMAND_SUCCESS);
}

t_command_status	ast_generate_lower_nodes(t_state *state, const char **ptr,
		t_node **daddy)
{
	t_command_status	status;
	char				*word;
	t_command			*command;
	const char			*cursor;

	cursor = *ptr;
	status = handle_word(NULL, &cursor, &word, &get_next_subcommand_pipe);
	if (status)
		return (status);
	command = command_create(word);
	ft_free_str(&word);
	if (!command)
		return (COMMAND_ERROR);
	if (*cursor == '|')
		return (ast_generate_lower_nodes_pipes(state, &cursor, daddy, command));
	*daddy = node_create(state, COMMAND, command);
	if (!*daddy)
		return (command_destroy(command), COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}
