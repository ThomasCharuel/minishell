/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:03:54 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 11:46:15 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command_status	get_next_subcommand(t_state *state, const char **ptr,
		t_list **words)
{
	t_command_status	status;
	char				*word;
	int					parenthesis_count;

	(void)state;
	parenthesis_count = 0;
	while (*ptr && **ptr)
	{
		if (!parenthesis_count && (!ft_strncmp(*ptr, "||", 2)
				|| !ft_strncmp(*ptr, "&&", 2)))
			break ;
		if (**ptr == '(')
			parenthesis_count++;
		else if (**ptr == ')')
			parenthesis_count--;
		if (ft_is_char_in_set(**ptr, "\'\""))
			status = get_next_word_char(ptr, &word, **ptr, true);
		else
			status = get_next_word(ptr, &word, "\'\"()&|", false);
		if (status)
			return (status);
		if (!str_list_append(words, word))
			return (ft_free_str(&word), COMMAND_ERROR);
	}
	return (COMMAND_SUCCESS);
}

static t_command_status	ast_generate_upper_nodes(t_state *s, const char **ptr)
{
	t_command_status	status;
	t_node				*node;
	char				*word;

	if (!ft_strncmp(*ptr, "||", 2))
		node = node_create(s, OR, NULL);
	else if (!ft_strncmp(*ptr, "&&", 2))
		node = node_create(s, AND, NULL);
	else
		return (print_error("syntax error", NULL), COMMAND_PARSING_ERROR);
	*ptr += 2;
	status = handle_word(NULL, ptr, &word, &get_next_subcommand);
	if (status)
		return (node_destroy(&node), status);
	status = ast_generate_lower_nodes(s, (const char **)&word, &node->right);
	ft_free_str(&word);
	if (status)
		return (node_destroy(&node), status);
	node->right->daddy = node;
	node->left = s->ast;
	s->ast->daddy = node;
	s->ast = node;
	if (**ptr)
		return (ast_generate_upper_nodes(s, ptr));
	return (COMMAND_SUCCESS);
}

// OK
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
	status = ast_generate_lower_nodes(state, (const char **)&word, &node);
	ft_free_str(&word);
	if (status)
		return (status);
	state->ast = node;
	if (*command_line)
		return (ast_generate_upper_nodes(state, &command_line));
	return (COMMAND_SUCCESS);
}
