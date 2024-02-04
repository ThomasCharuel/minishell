/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/04 19:47:54 by romain           ###   ########.fr       */
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

t_command_status	exec_command(t_state *state, t_command *command, int fd[2])
{
	t_command_status	parsing_status;

	// command = command_create(command_str);
	if (!command)
		return (COMMAND_ERROR);
	parsing_status = command_parse(state, command);
	if (parsing_status != COMMAND_SUCCESS)
		return (command_destroy(&command), parsing_status);
	command->pid = fork();
	if (command->pid == -1)
		return (perror("minishell"), command_destroy(&command), COMMAND_ERROR);
	if (command->pid == 0)
	{
		dup2(fd[IN_FD], STDIN_FILENO);
		dup2(fd[OUT_FD], STDOUT_FILENO);
		if (fd[IN_FD] != STDIN_FILENO)
			close(fd[IN_FD]);
		if (fd[OUT_FD] != STDIN_FILENO)
			close(fd[OUT_FD]);
		execve(command->argv[0], command->argv, state->envp);
	}
	waitpid(command->pid, &command->status, 0);
	if (WIFEXITED(command->status))
		parsing_status = WEXITSTATUS(command->status);
	command_destroy(&command);
	return (COMMAND_SUCCESS);
}

t_command_status	exec_line(t_state *state, const char *line)
{
	char **commands;
	int i;
	int *pipefd;
	int fd[2];
	int len;

	commands = ft_split(line, '|');
	if (!commands)
		return (COMMAND_ERROR);
	len = ft_strslen(commands);
	pipefd = malloc(sizeof(int) * len * 2);
	if (!commands)
		return (COMMAND_ERROR);
	while (commands[i])
	{
		if (i < len)
		{
			pipe(&pipefd[i * 2]);
			exec_command(state, commands[i++], &pipefd[i*2 + 1]);
		}
		ft_free_strs(commands);
	}
}
