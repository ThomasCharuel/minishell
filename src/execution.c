/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/05 11:42:12 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_input_redirect(char *in_file_path)
{
	int	in;

	in = open(in_file_path, O_RDONLY);
	if (dup2(in, STDIN_FILENO) == -1)
	{
		close(in);
		exit(EXIT_FAILURE);
	}
	close(in);
}

t_return_status	exec_command(t_state *state, t_command *command)
{
	command->pid = fork();
	if (command->pid == -1)
		return (perror("minishell"), ERROR);
	if (command->pid == 0)
	{
		dup2(command->in_fd, STDIN_FILENO);
		dup2(command->out_fd, STDOUT_FILENO);
		if (command->in_fd != STDIN_FILENO)
			close(command->in_fd);
		if (command->out_fd != STDOUT_FILENO)
			close(command->out_fd);
		execve(command->argv[0], command->argv, state->envp);
	}
	if (command->in_fd != STDIN_FILENO)
		close(command->in_fd);
	if (command->out_fd != STDOUT_FILENO)
		close(command->out_fd);
	return (SUCCESS);
}

t_command	**parsing(t_state *state, const char *line)
{
	char		**command_strs;
	t_command	**commands;
	size_t		i;

	command_strs = ft_split(line, '|');
	if (!command_strs)
		return (NULL);
	commands = ft_calloc(ft_strslen((const void **)command_strs) + 1,
			sizeof(t_command *));
	if (!commands)
		return (ft_free_strs((void **)command_strs), NULL);
	i = 0;
	while (command_strs[i])
	{
		commands[i] = command_create(command_strs[i]);
		if (!commands[i] || command_parse(state, commands[i]) == COMMAND_ERROR)
			return (ft_free_strs((void **)commands),
				ft_free_strs((void **)command_strs), NULL);
		i++;
	}
	return (commands);
}

t_command_status	exec_line(t_state *state, const char *line)
{
	t_command			**commands;
	size_t				i;
	t_command_status	status;
	t_pipe				*pipes;
	size_t				len;

	commands = parsing(state, line);
	if (!commands)
		return (COMMAND_ERROR);
	len = ft_strslen((const void **)commands);
	pipes = calloc(len, sizeof(t_pipe));
	if (!pipes)
		return (ft_free_strs((void **)commands), COMMAND_ERROR);
	i = 0;
	while (commands[i])
	{
		if (i < len - 1)
		{
			pipe(pipes[i].fd);
			commands[i]->out_fd = pipes[i].fd[OUT_FD];
		}
		if (i > 0)
			commands[i]->in_fd = pipes[i - 1].fd[IN_FD];
		if (!exec_command(state, commands[i]))
			printf("TODO\n"); // TODO: il faut cleanup tout correctement
		i++;
	}
	i--;
	waitpid(commands[i]->pid, &(commands[i]->status), 0);
	status = WIFEXITED(commands[i]->status) && WEXITSTATUS(commands[i]->status);
	while (wait(NULL) != -1)
		continue ;
	i = 0;
	while (commands[i])
		command_destroy(&commands[i++]);
	ft_free_strs((void **)commands);
	return (status);
}
