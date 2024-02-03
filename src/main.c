/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 19:24:23 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_code;

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

t_return_code	ft_exec(t_state *state, const char *command)
{
	const char	*path;
	char		*argv[2] = {NULL, NULL};
	pid_t		pid;
	int			status;

	path = envp_get((const char **)state->envp, "PATH");
	if (!path)
		return (ERROR);
	argv[0] = get_command_file(path, command);
	if (!argv[0])
	{
		write(STDERR_FILENO, command, ft_strlen(command));
		write(STDERR_FILENO, ": command not found\n", 20);
		state->last_exit_code = COMMAND_NOT_FOUND;
		return (SUCCESS);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("bash");
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

int	main(int argc, char **argv, const char **envp)
{
	char	*line;
	t_state	*state;

	(void)argc;
	(void)argv;
	signal_init();
	state = state_init(envp);
	if (!state)
		return (1);
	while (g_signal_code != SIGTERM)
	{
		line = prompt_loop();
		if (!line)
			break ;
		if (!is_whitespace_line(line))
		{
			add_history(line);
			if (!ft_exec(state, line))
			{
				free(line);
				break ;
			}
		}
		free(line);
	}
	state_cleanup(state);
	return (0);
}
