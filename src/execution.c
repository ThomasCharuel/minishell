/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/14 13:21:01 by tcharuel         ###   ########.fr       */
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

t_command_status	command_exec(t_state *state, t_node *node)
{
	char		**argv;
	t_command	*command;

	command = node->content;
	argv = from_list_to_array(command->argv);
	if (!argv)
		return (COMMAND_ERROR);
	if (is_builtin(argv[0]))
		return (state->last_child_pid = 0, builtin_exec(state, node, argv));
	state->last_child_pid = fork();
	if (state->last_child_pid == -1)
		return (perror("minishell"), COMMAND_ERROR);
	if (state->last_child_pid == 0)
	{
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
	status = get_next_command(&cursor, &word);
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
		status = get_next_expression(cursor, &word);
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
		status = get_next_expression(cursor, &word);
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
	ft_printf("Files filtered: \n");
	display_str_list(filter_files_based_on_pattern("t*st"));
	return (COMMAND_SUCCESS);
}

t_command_status	line_exec(t_state *state, const char *line)
{
	int					command_status;
	t_command_status	status;

	status = line_parsing(state, line);
	if (status)
		return (status);
	status = ast_execute(state, state->ast);
	if (status)
		state->last_exit_code = status;
	else
	{
		if (state->last_child_pid)
		{
			// To be refactored (check other waitpid codes)
			waitpid(state->last_child_pid, &command_status, 0);
			state->last_exit_code = WIFEXITED(command_status)
				&& WEXITSTATUS(command_status);
		}
	}
	while (wait(NULL) != -1)
		continue ;
	node_destroy(&state->ast);
	ft_lstclear(&state->heredocs, &heredoc_destroy);
	return (status);
}
