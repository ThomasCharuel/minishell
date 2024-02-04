/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/04 17:22:04 by tcharuel         ###   ########.fr       */
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

t_command_status	exec_line(t_state *state, const char *command_str)
{
	t_command			*command;
	t_command_status	parsing_status;

	command = command_create(command_str);
	if (!command)
		return (COMMAND_ERROR);
	parsing_status = command_parse(state, command);
	if (parsing_status != COMMAND_SUCCESS)
		return (command_destroy(&command), parsing_status);
	command->pid = fork();
	if (command->pid == -1)
		return (perror("minishell"), command_destroy(&command), COMMAND_ERROR);
	if (command->pid == 0)
		execve(command->argv[0], command->argv, state->envp);
	waitpid(command->pid, &command->status, 0);
	if (WIFEXITED(command->status))
		parsing_status = WEXITSTATUS(command->status);
	command_destroy(&command);
	return (COMMAND_SUCCESS);
}
