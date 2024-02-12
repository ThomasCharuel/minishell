/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/12 14:58:11 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirections(t_node *node)
{
	t_list			*argv_node;
	t_redirection	*redirection;
	t_command		*command;

	command = node->content;
	argv_node = command->redirections;
	while (argv_node)
	{
		redirection = argv_node->content;
		if (redirection->type == WRITE || redirection->type == APPEND)
		{
			if (redirection->type == WRITE)
				node->write_fd = open(redirection->file,
						O_WRONLY | O_CREAT | O_TRUNC,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			else if (redirection->type == APPEND)
				node->write_fd = open(redirection->file,
						O_WRONLY | O_CREAT | O_APPEND,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(node->write_fd, STDOUT_FILENO);
			ft_close_fd(node->write_fd);
		}
		else if (redirection->type == READ)
		{
			node->read_fd = open(redirection->file, O_RDONLY);
			dup2(node->read_fd, STDIN_FILENO);
			ft_close_fd(node->read_fd);
		}
		argv_node = argv_node->next;
	}
}

char	**from_list_to_array(t_list *list)
{
	char	**strs;
	size_t	i;

	i = ft_lst_len(list);
	strs = calloc(i + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	i = 0;
	while (list)
	{
		strs[i++] = list->content;
		list = list->next;
	}
	return (strs);
}

int	get_fd_to_close(t_node *node)
{
	t_node	*daddy;

	daddy = node->daddy;
	if (daddy && daddy->type == PIPE && daddy->right != node)
		return (((t_pipe *)daddy->content)->fd[IN_FD]);
	return (STDIN_FILENO);
}

t_return_status	command_exec(t_state *state, t_node *node)
{
	char		**argv;
	t_command	*command;

	command = node->content;
	argv = from_list_to_array(command->argv);
	if (!argv)
		return (ERROR);
	command->pid = fork();
	if (command->pid == -1)
		return (perror("minishell"), ERROR);
	if (command->pid == 0)
	{
		ft_printf("CMD: %s\n", argv[0]);
		ft_printf("FD to close: %d, FD in: %d, FD out: %d\n",
			get_fd_to_close(node), node->read_fd, node->write_fd);
		ft_close_fd(get_fd_to_close(node));
		dup2(node->read_fd, STDIN_FILENO);
		ft_close_fd(node->read_fd);
		dup2(node->write_fd, STDOUT_FILENO);
		ft_close_fd(node->write_fd);
		handle_redirections(node);
		execve(argv[0], argv, state->envp);
		// state cleanup
	}
	free(argv);
	ft_close_fd(node->read_fd);
	ft_close_fd(node->write_fd);
	return (SUCCESS);
}

// t_command_status	line_parsing_old(t_state *state, const char *line)
// {
// 	char	**command_strs;
// 	size_t	i;

// 	state->commands = ft_calloc(ft_strslen((const void **)command_strs) + 1,
// 			sizeof(t_command *));
// 	if (!state->commands)
// 		return (ft_clean_double_list((void **)command_strs, free),
// 			COMMAND_ERROR);
// 	i = 0;
// 	while (command_strs[i])
// 	{
// 		state->commands[i] = command_create(command_strs[i]);
// 		if (!state->commands[i] || command_handle_heredocs(state,
// 				state->commands[i]) || command_parse(state,
// 				state->commands[i]) == COMMAND_ERROR)
// 			return (ft_clean_double_list((void **)command_strs, free),
// 				COMMAND_ERROR);
// 		i++;
// 	}
// 	ft_clean_double_list((void **)command_strs, free);
// 	return (COMMAND_SUCCESS);
// }

t_command_status	parse_next_line_block(const char **ptr, char **res)
{
	t_command_status	status;
	t_list				*words;
	char				*word;
	const char			*cursor;
	bool				parenthesis;

	parenthesis = false;
	cursor = *ptr;
	words = NULL;
	while (*cursor)
	{
		if (!parenthesis && (!ft_strncmp(cursor, "||", 2) || !ft_strncmp(cursor,
					"&&", 2)))
			break ;
		if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"", true);
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else
		{
			if (*cursor == '(')
				parenthesis = true;
			else if (*cursor == ')')
				parenthesis = false;
			status = get_next_word_new(&cursor, &word, "\'\"()&|", false);
		}
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
	}
	*res = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	*ptr = cursor;
	return (COMMAND_SUCCESS);
}

t_command_status	parse_next_line_command(const char **ptr, char **res)
{
	t_command_status	status;
	t_list				*words;
	char				*word;
	const char			*cursor;
	bool				parenthesis;

	parenthesis = false;
	cursor = *ptr;
	words = NULL;
	while (*cursor)
	{
		if (!parenthesis && *cursor == '|')
			break ;
		if (*cursor == '\"')
			status = get_next_word_new(&cursor, &word, "\"", true);
		else if (*cursor == '\'')
			status = get_next_word_new(&cursor, &word, "\'", true);
		else
		{
			if (*cursor == '(')
				parenthesis = true;
			else if (*cursor == ')')
				parenthesis = false;
			status = get_next_word_new(&cursor, &word, "\'\"()|", false);
		}
		if (status)
			return (ft_lstclear(&words, free), status);
		if (!str_list_append(&words, word))
			return (ft_lstclear(&words, free), COMMAND_ERROR);
	}
	*res = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	*ptr = cursor;
	return (COMMAND_SUCCESS);
}

t_command_status	command_generation_handling(const char **ptr,
		t_node **daddy)
{
	t_command_status	status;
	char				*word;
	t_command			*command;
	const char			*cursor;

	cursor = *ptr;
	status = parse_next_line_command(&cursor, &word);
	(void)status; // handle status
	command = command_create(word);
	free(word);
	if (*cursor == '|')
	{
		*daddy = node_create(PIPE, NULL);
		(*daddy)->left = node_create(COMMAND, command);
		(*daddy)->left->daddy = *daddy;
		cursor++;
		status = command_generation_handling(&cursor, &(*daddy)->right);
		(*daddy)->right->daddy = *daddy;
		(void)status; // handle status
	}
	else
		*daddy = node_create(COMMAND, command);
	return (COMMAND_SUCCESS);
}

t_command_status	ast_generate(t_state *state, const char **cursor,
		t_node *left_child)
{
	t_command_status	status;
	t_node				*node;
	char				*word;

	if (!left_child)
	{
		status = parse_next_line_block(cursor, &word);
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
		status = parse_next_line_block(cursor, &word);
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
		return (ast_generate(state, cursor, node));
	return (COMMAND_SUCCESS);
}

t_command_status	line_parsing(t_state *state, const char *line)
{
	t_command_status	status;
	const char			*cursor;

	status = handle_heredocs(state, line);
	if (status)
		return (status);
	cursor = state->line;
	status = ast_generate(state, &cursor, NULL);
	ft_free_str(&state->line);
	return (COMMAND_SUCCESS);
}

t_command_status	line_exec(t_state *state, const char *line)
{
	t_command_status	status;

	status = line_parsing(state, line);
	if (status)
		return (status);
	ast_execute(state, state->ast);
	while (wait(NULL) != -1)
		// Mettre pid dans une liste chaine et get le derniere status
		continue ;
	node_destroy(&state->ast);
	ft_lstclear(&state->heredocs, &heredoc_destroy);
	return (status);
	// size_t				i;
	// size_t				len;
	// int					fd_to_close;
	// len = ft_strslen((const void **)state->commands);
	// state->pipes = calloc(len, sizeof(t_pipe));
	// if (!state->pipes)
	// 	return (COMMAND_ERROR);
	// i = 0;
	// while (state->commands[i])
	// {
	// 	if (i < len - 1)
	// 	{
	// 		pipe(state->pipes[i].fd);
	// 		state->commands[i]->out_fd = state->pipes[i].fd[OUT_FD];
	// 		fd_to_close = state->pipes[i].fd[IN_FD];
	// 	}
	// 	else
	// 		fd_to_close = STDIN_FILENO;
	// 	if (i > 0)
	// 		state->commands[i]->in_fd = state->pipes[i - 1].fd[IN_FD];
	// 	if (!command_exec(state, state->commands[i], fd_to_close))
	// 		printf("TODO\n"); // TODO: il faut cleanup tout correctement
	// 	i++;
	// }
	// i--;
	// waitpid(state->commands[i]->pid, &(state->commands[i]->status), 0);
	// status = WIFEXITED(state->commands[i]->status)
	// 	&& WEXITSTATUS(state->commands[i]->status);
	// while (wait(NULL) != -1)
	// 	continue ;
}
