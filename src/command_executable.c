/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 14:41:51 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 13:52:00 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static char	**get_paths(t_state *state)
{
	const char	*path;
	char		**paths;

	path = envp_get(state, "PATH");
	paths = ft_split(path, ':');
	if (!paths)
		return (perror("minishell"), NULL);
	return (paths);
}

// OK
static t_command_status	handle_command(t_state *state, t_command *command)
{
	char	**paths;
	char	*norm;
	size_t	i;
	char	*command_path;

	norm = command->argv->content;
	if (!norm || *norm == '\0')
		return (print_error("Command '' not found", NULL), COMMAND_NOT_FOUND);
	paths = get_paths(state);
	if (!paths)
		return (perror("minishell"), COMMAND_ERROR);
	i = 0;
	while (paths[i])
	{
		command_path = ft_strsjoin(paths[i++], "/", norm, NULL);
		if (!command_path)
			return (perror("minishell"), ft_clean_double_list((void **)paths,
					free), COMMAND_ERROR);
		if (!access(command_path, X_OK))
			return (ft_clean_double_list((void **)paths, free), free(norm),
				command->argv->content = command_path, COMMAND_SUCCESS);
		free(command_path);
	}
	return (ft_clean_double_list((void **)paths, free), print_error(norm,
			": command not found", NULL), COMMAND_NOT_FOUND);
}

// OK
static t_command_status	handle_path_command(t_command *command)
{
	char	*command_str;

	command_str = command->argv->content;
	if (command_str[ft_strlen(command_str) - 1] == '/'
		|| command_str[ft_strlen(command_str) - 1] == '.')
		return (print_error(command_str, ": is a directory", NULL),
			COMMAND_ISDIRECTORY);
	if (access(command_str, R_OK) == -1)
		return (print_error(command_str, ": No such file or directory", NULL),
			COMMAND_NOT_FOUND);
	if (access(command_str, X_OK) == -1)
		return (print_error(command_str, ": Permission denied", NULL),
			COMMAND_NOT_EXECUTABLE);
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	set_command_executable(t_state *state, t_command *command)
{
	if (!command->argv)
		return (COMMAND_NOT_FOUND);
	if (is_builtin(command->argv->content))
		return (COMMAND_SUCCESS);
	if (ft_strchr((char *)command->argv->content, '/'))
		return (handle_path_command(command));
	return (handle_command(state, command));
}
