/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 14:41:51 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/05 11:05:48 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command_file(const char *path, const char *cmd)
{
	char	**paths;
	char	*command_file;
	int		i;

	paths = ft_split(path, ':');
	if (!paths)
		return (perror("minishell"), NULL);
	i = 0;
	while (paths[i])
	{
		command_file = ft_strsjoin(paths[i], "/", cmd, NULL);
		if (!command_file)
			return (perror("minishell"), ft_free_strs((void **)paths), NULL);
		if (!access(command_file, X_OK))
			return (ft_free_strs((void **)paths), command_file);
		free(command_file);
		i++;
	}
	return (ft_free_strs((void **)paths), NULL);
}

t_command_status	handle_command(t_state *state, t_command *command)
{
	const char	*path;
	char		**paths;
	size_t		i;
	char		*command_path;

	path = envp_get((const char **)state->envp, "PATH");
	if (!path)
		return (COMMAND_NOT_FOUND);
	paths = ft_split(path, ':');
	if (!paths)
		return (perror("minishell"), COMMAND_ERROR);
	i = 0;
	while (paths[i])
	{
		command_path = ft_strsjoin(paths[i], "/", command->argv[0], NULL);
		if (!command_path)
			return (perror("minishell"), ft_free_strs((void **)paths),
				COMMAND_ERROR);
		if (!access(command_path, X_OK))
			return (ft_free_strs((void **)paths), free(command->argv[0]),
				command->argv[0] = command_path, COMMAND_SUCCESS);
		free(command_path);
		i++;
	}
	write(STDERR_FILENO, command->argv[0], ft_strlen(command->argv[0]));
	write(STDERR_FILENO, ": command not found\n", 21);
	return (ft_free_strs((void **)paths), COMMAND_NOT_FOUND);
}

t_command_status	handle_path_command(t_command *command)
{
	char	*err_message;

	if (access(command->argv[0], X_OK))
	{
		err_message = ft_strsjoin("minishell: ", command->argv[0],
				": No such file or directory\n", NULL);
		if (!err_message)
			return (perror("minishell"), COMMAND_ERROR);
		write(STDERR_FILENO, err_message, ft_strlen(err_message));
		free(err_message);
		return (COMMAND_NOT_FOUND);
	}
	return (COMMAND_SUCCESS);
}
