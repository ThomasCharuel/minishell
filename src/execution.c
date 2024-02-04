/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/04 20:42:00 by tcharuel         ###   ########.fr       */
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
		dup2(command->fd[IN_FD], STDIN_FILENO);
		dup2(command->fd[OUT_FD], STDOUT_FILENO);
		if (command->fd[IN_FD] != STDIN_FILENO)
			close(command->fd[IN_FD]);
		if (command->fd[OUT_FD] != STDOUT_FILENO)
			close(command->fd[OUT_FD]);
		execve(command->argv[0], command->argv, state->envp);
	}
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
	commands = ft_calloc(ft_strslen((const char **)command_strs) + 1,
			sizeof(t_command *));
	if (!commands)
		return (ft_free_strs(command_strs), NULL);
	i = 0;
	while (command_strs[i])
	{
		commands[i] = command_create(command_strs[i]);
		if (!commands[i] || command_parse(state, commands[i]) == COMMAND_ERROR)
			return (ft_free_strs((char **)commands), ft_free_strs(command_strs),
				NULL);
		i++;
	}
	return (commands);
}

t_command_status	exec_line(t_state *state, const char *line)
{
	t_command			**commands;
	size_t				i;
	t_command_status	status;

	// int			*pipefd;
	// int			len;
	commands = parsing(state, line);
	if (!commands)
		return (COMMAND_ERROR);
	// len = ft_strslen(commands);
	// pipefd = malloc(sizeof(int) * len * 2);
	i = 0;
	while (commands[i])
	{
		// if (i < len)
		// {
		// 	pipe(&pipefd[i * 2]);
		// 	exec_command(state, commands[i++], &pipefd[i * 2 + 1]);
		// }
		// ft_free_strs(commands);
		if (!exec_command(state, commands[i]))
			printf("TODO\n"); // TODO: il faut cleanup tout correctement
		i++;
	}
	i--;
	waitpid(commands[i]->pid, &(commands[i]->status), 0);
	if (WIFEXITED(commands[i]->status))
		status = WEXITSTATUS(commands[i]->status);
	while (wait(NULL) != -1)
		continue ;
	i = 0;
	while (commands[i])
		command_destroy(&commands[i++]);
	free(commands);
	return (status);
}
