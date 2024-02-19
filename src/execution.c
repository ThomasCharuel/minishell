/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/19 18:35:35 by tcharuel         ###   ########.fr       */
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
	strs = ft_calloc(i + 1, sizeof(char *));
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

t_command_status	subshell_execute(t_state *state, t_node *node)
{
	char		*word;
	char		*instruction;
	t_command	*command;

	command = node->content;
	word = ft_strdup(state->executable_path);
	if (!word)
		return (COMMAND_ERROR);
	if (!str_list_append(&command->argv, word))
		return (free(word), COMMAND_ERROR);
	instruction = ft_strdup(command->command_str);
	if (!instruction)
		return (COMMAND_ERROR);
	if (!str_list_append(&command->argv, instruction))
		return (free(instruction), COMMAND_ERROR);
	return (command_exec(state, node));
}

t_command_status	command_generation_handling(const char **ptr,
		t_node **daddy)
{
	t_command_status	status;
	char				*word;
	t_command			*command;
	const char			*cursor;

	cursor = *ptr;
	status = handle_word(NULL, &cursor, &word, &get_next_command);
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
