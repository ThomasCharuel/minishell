/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/06 18:55:57 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirections(t_command *command)
{
	t_list			*node;
	t_redirection	*redirection;

	node = command->redirections;
	while (node)
	{
		redirection = node->content;
		if (redirection->type == WRITE || redirection->type == APPEND)
		{
			if (redirection->type == WRITE)
				command->out_fd = open(redirection->file,
						O_WRONLY | O_CREAT | O_TRUNC,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			else if (redirection->type == APPEND)
				command->out_fd = open(redirection->file,
						O_WRONLY | O_CREAT | O_APPEND,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(command->out_fd, STDOUT_FILENO);
			ft_close_fd(command->out_fd);
		}
		else if (redirection->type == READ)
		{
			command->in_fd = open(redirection->file, O_RDONLY);
			dup2(command->in_fd, STDIN_FILENO);
			ft_close_fd(command->in_fd);
		}
		node = node->next;
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

t_return_status	command_exec(t_state *state, t_command *command,
		int fd_to_close)
{
	char	**argv;

	argv = from_list_to_array(command->argv);
	if (!argv)
		return (ERROR);
	command->pid = fork();
	if (command->pid == -1)
		return (perror("minishell"), ERROR);
	if (command->pid == 0)
	{
		ft_close_fd(fd_to_close);
		dup2(command->in_fd, STDIN_FILENO);
		ft_close_fd(command->in_fd);
		dup2(command->out_fd, STDOUT_FILENO);
		ft_close_fd(command->out_fd);
		handle_redirections(command);
		execve(argv[0], argv, state->envp);
	}
	free(argv);
	ft_close_fd(command->in_fd);
	ft_close_fd(command->out_fd);
	return (SUCCESS);
}

t_command_status	line_parsing(t_state *state, const char *line)
{
	char	**command_strs;
	size_t	i;

	command_strs = ft_split(line, '|');
	if (!command_strs)
		return (COMMAND_ERROR);
	state->commands = ft_calloc(ft_strslen((const void **)command_strs) + 1,
			sizeof(t_command *));
	if (!state->commands)
		return (ft_clean_double_list((void **)command_strs, free),
			COMMAND_ERROR);
	i = 0;
	while (command_strs[i])
	{
		state->commands[i] = command_create(command_strs[i]);
		if (!state->commands[i] || command_parse(state,
				state->commands[i]) == COMMAND_ERROR)
			return (ft_clean_double_list((void **)command_strs, free),
				COMMAND_ERROR);
		i++;
	}
	ft_clean_double_list((void **)command_strs, free);
	return (COMMAND_SUCCESS);
}

t_command_status	line_exec(t_state *state, const char *line)
{
	size_t				i;
	t_command_status	status;
	size_t				len;
	int					fd_to_close;

	if (line_parsing(state, line) == COMMAND_ERROR || !state->commands)
		return (COMMAND_ERROR);
	len = ft_strslen((const void **)state->commands);
	state->pipes = calloc(len, sizeof(t_pipe));
	if (!state->pipes)
		return (COMMAND_ERROR);
	i = 0;
	while (state->commands[i])
	{
		if (i < len - 1)
		{
			pipe(state->pipes[i].fd);
			state->commands[i]->out_fd = state->pipes[i].fd[OUT_FD];
			fd_to_close = state->pipes[i].fd[IN_FD];
		}
		else
			fd_to_close = STDIN_FILENO;
		if (i > 0)
			state->commands[i]->in_fd = state->pipes[i - 1].fd[IN_FD];
		if (!command_exec(state, state->commands[i], fd_to_close))
			printf("TODO\n"); // TODO: il faut cleanup tout correctement
		i++;
	}
	i--;
	waitpid(state->commands[i]->pid, &(state->commands[i]->status), 0);
	status = WIFEXITED(state->commands[i]->status)
		&& WEXITSTATUS(state->commands[i]->status);
	while (wait(NULL) != -1)
		continue ;
	ast_cleanup(state);
	return (status);
}
