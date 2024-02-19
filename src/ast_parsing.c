/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:03:54 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/19 19:58:06 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command_status	get_next_subcommand(t_state *state, const char **ptr,
		t_list **words)
{
	t_command_status	status;
	char				*word;
	bool				parenthesis;

	(void)state;
	parenthesis = false;
	while (*ptr && **ptr)
	{
		if (!parenthesis && (!ft_strncmp(*ptr, "||", 2) || !ft_strncmp(*ptr,
					"&&", 2)))
			break ;
		if (**ptr == '(')
			parenthesis = true;
		else if (**ptr == ')')
			parenthesis = false;
		if (ft_is_char_in_set(**ptr, "\'\""))
			status = get_next_word_char(ptr, &word, **ptr, true);
		else
			status = get_next_word(ptr, &word, "\'\"()&|", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}

static t_command_status	ast_generate_upper_nodes(t_state *state,
		const char **ptr)
{
	t_command_status	status;
	t_node				*node;
	char				*word;

	if (!ft_strncmp(*ptr, "||", 2))
		node = node_create(OR, NULL);
	else if (!ft_strncmp(*ptr, "&&", 2))
		node = node_create(AND, NULL);
	else
		return (COMMAND_PARSING_ERROR);
	*ptr += 2;
	status = handle_word(NULL, ptr, &word, &get_next_subcommand);
	if (status)
		return (status);
	ft_printf("Upper nodes Word: '%s'\n", word);
	status = ast_generate_lower_nodes((const char **)&word, &node->right);
	free(word);
	if (status)
		return (status);
	node->right->daddy = node;
	node->left = state->ast;
	state->ast->daddy = node;
	state->ast = node;
	if (**ptr)
		return (ast_generate_upper_nodes(state, ptr));
	return (COMMAND_SUCCESS);
}

t_command_status	ast_generate(t_state *state)
{
	const char			*command_line;
	t_command_status	status;
	char				*word;
	t_node				*node;

	command_line = state->line;
	status = handle_word(NULL, &command_line, &word, &get_next_subcommand);
	if (status)
		return (status);
	ft_printf("AST Word: '%s'\n", word);
	status = ast_generate_lower_nodes((const char **)&word, &node);
	free(word);
	if (status)
		return (status);
	state->ast = node;
	if (*command_line)
		return (ast_generate_upper_nodes(state, &command_line));
	return (COMMAND_SUCCESS);
}
