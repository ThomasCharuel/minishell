/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/02 20:11:23 by tcharuel         ###   ########.fr       */
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

void	ft_exec(char *command, char **envp)
{
	char			*argv[] = {command, NULL};
	char			**path;
	struct dirent	*file;

	path = ft_split(get_var(data, "PATH")->value, ':');
	file = read_dir(path, command);
	execve(file->d_name, argv, envp);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	signal_init();
	while (true)
	{
		line = prompt_loop();
		if (!line)
			break ;
		if (!is_whitespace_line(line))
		{
			add_history(line);
			ft_exec(line, envp);
		}
		free(line);
	}
	ft_printf("exit\n");
	return (0);
}
