/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 14:17:02 by tcharuel         ###   ########.fr       */
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

struct dirent	*read_dir(char **path, char *cmd)
{
	struct dirent	*file;
	DIR				*dir;
	int				i;

	i = 0;
	dir = opendir(path[i++]);
	while (dir)
	{
		file = readdir(dir);
		while (file)
		{
			if (!ft_strncmp(file->d_name, cmd, ft_strlen(file->d_name)))
				return (file);
			file = readdir(dir);
		}
		dir = opendir(path[i++]);
	}
	return (NULL);
}

t_return_code	ft_exec(t_state *state, char *command)
{
	char			*argv[] = {command, NULL};
	char			**path;
	struct dirent	*file;

	path = ft_split(ft_dict_get(state->vars, "PATH"), ':');
	if (!path)
		return (ERROR);
	file = read_dir(path, command);
	if (fork())
		execve(file->d_name, argv, envp);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
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
