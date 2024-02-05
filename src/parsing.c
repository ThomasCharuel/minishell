/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 14:41:51 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/05 19:46:25 by tcharuel         ###   ########.fr       */
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
			return (perror("minishell"), ft_clean_double_list((void **)paths,
					free), NULL);
		if (!access(command_file, X_OK))
			return (ft_clean_double_list((void **)paths, free), command_file);
		free(command_file);
		i++;
	}
	return (ft_clean_double_list((void **)paths, free), NULL);
}

t_command_status	handle_command(t_state *state, t_command *command)
{
	const char	*path;
	char		**paths;
	size_t		i;
	char		*command_path;
	char		*command_str;

	command_str = (char *)command->argv->data;
	path = envp_get((const char **)state->envp, "PATH");
	if (!path)
		return (COMMAND_NOT_FOUND);
	paths = ft_split(path, ':');
	if (!paths)
		return (perror("minishell"), COMMAND_ERROR);
	i = 0;
	while (paths[i])
	{
		command_path = ft_strsjoin(paths[i], "/", command_str, NULL);
		if (!command_path)
			return (perror("minishell"), ft_clean_double_list((void **)paths,
					free), COMMAND_ERROR);
		if (!access(command_path, X_OK))
			return (ft_clean_double_list((void **)paths, free),
				free(command_str), command_str = command_path, COMMAND_SUCCESS);
		free(command_path);
		i++;
	}
	write(STDERR_FILENO, command_str, ft_strlen(command_str));
	write(STDERR_FILENO, ": command not found\n", 21);
	return (ft_clean_double_list((void **)paths, free), COMMAND_NOT_FOUND);
}

t_command_status	handle_path_command(t_command *command)
{
	char	*err_message;
	char	*command_str;

	command_str = (char *)command->argv->data;
	if (access(command_str, X_OK))
	{
		err_message = ft_strsjoin("minishell: ", command_str,
				": No such file or directory\n", NULL);
		if (!err_message)
			return (perror("minishell"), COMMAND_ERROR);
		write(STDERR_FILENO, err_message, ft_strlen(err_message));
		free(err_message);
		return (COMMAND_NOT_FOUND);
	}
	return (COMMAND_SUCCESS);
}
