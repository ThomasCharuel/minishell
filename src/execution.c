/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:24:52 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 21:03:43 by tcharuel         ###   ########.fr       */
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

char	*get_command_file(const char *path, const char *cmd)
{
	char	**paths;
	char	*command_file;
	int		i;

	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		command_file = ft_strsjoin(paths[i], "/", cmd, NULL);
		if (!access(command_file, X_OK))
			return (ft_free_strs(paths), command_file);
		free(command_file);
		i++;
	}
	return (ft_free_strs(paths), NULL);
}

t_return_code	handle_command(t_state *state, const char *command,
		char **command_path)
{
	const char	*path;
	char		**paths;
	size_t		i;

	path = envp_get((const char **)state->envp, "PATH");
	if (!path)
		return (ERROR);
	paths = ft_split(path, ':');
	if (!paths)
		return (ERROR);
	i = 0;
	while (paths[i])
	{
		*command_path = ft_strsjoin(paths[i], "/", command, NULL);
		if (!access(*command_path, X_OK))
			return (ft_free_strs(paths), SUCCESS);
		free(*command_path);
		i++;
	}
	*command_path = NULL;
	write(STDERR_FILENO, command, ft_strlen(command));
	write(STDERR_FILENO, ": command not found\n", 21);
	return (ft_free_strs(paths), SUCCESS);
}

t_return_code	handle_path_command(const char *command, char **command_path)
{
	char	*err_message;

	*command_path = ft_strdup(command);
	if (!*command_path)
		return (ERROR);
	if (access(*command_path, F_OK))
	{
		err_message = ft_strsjoin("minishell: ", command,
				": No such file or directory\n", NULL);
		write(STDERR_FILENO, err_message, ft_strlen(err_message));
		free(err_message);
		free(*command_path);
		*command_path = NULL;
	}
	return (SUCCESS);
}

t_return_code	ft_exec(t_state *state, const char *command_str)
{
	t_command	*command;
	pid_t		pid;
	int			status;

	command = command_create(command_str);
	if (!command)
		return (ERROR);
	if (ft_strchr(command, '/'))
	{
		if (!handle_path_command(command, &argv[0]))
			return (perror("minishell"), ERROR);
	}
	else if (!handle_command(state, command, &argv[0]))
		return (perror("minishell"), ERROR);
	if (!argv[0])
		return (state->last_exit_code = COMMAND_NOT_FOUND, SUCCESS);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (free(argv[0]), ERROR);
	}
	if (pid == 0)
		execve(argv[0], argv, state->envp);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->last_exit_code = WEXITSTATUS(status);
	free(argv[0]);
	return (SUCCESS);
}
