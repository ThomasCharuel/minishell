/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 16:01:17 by tcharuel         ###   ########.fr       */
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

struct dirent	*read_dir(const char *path, const char *cmd)
{
	char			**paths;
	struct dirent	*file;
	DIR				*dir;
	int				i;

	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	dir = opendir(paths[i++]);
	while (dir)
	{
		file = readdir(dir);
		while (file)
		{
			if (!ft_strncmp(file->d_name, cmd, ft_strlen(file->d_name)))
				return (file);
			file = readdir(dir);
		}
		dir = opendir(paths[i++]);
	}
	return (NULL);
}

t_return_code	ft_exec(t_state *state, const char *command)
{
	const char		*path;
	struct dirent	*file;
	char			*argv[] = {(char *)command, NULL};

	path = envp_get((const char **)state->envp, "PATH");
	if (!path)
		return (ERROR);
	file = read_dir(path, command);
	if (!file)
	{
		write(STDERR_FILENO, command, ft_strlen(command));
		write(STDERR_FILENO, ": command not found\n", 20);
		state->last_exit_code = COMMAND_NOT_FOUND;
		return (SUCCESS);
	}
	if (fork())
		execve(file->d_name, argv, state->envp);
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
