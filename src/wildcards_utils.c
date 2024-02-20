/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:15:01 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/20 17:01:18 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command_status	init_files_in_current_directory(t_list **files,
		DIR **dir)
{
	char	*pwd;

	*files = NULL;
	*dir = NULL;
	pwd = get_working_directory();
	if (!pwd)
		return (COMMAND_ERROR);
	if (!*pwd)
		return (COMMAND_SUCCESS);
	*dir = opendir(pwd);
	free(pwd);
	if (!dir)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

t_command_status	get_files_in_current_directory(t_list **files)
{
	t_command_status	status;
	DIR					*dir;
	struct dirent		*file;
	char				*filename;

	status = init_files_in_current_directory(files, &dir);
	if (status || !dir)
		return (status);
	file = readdir(dir);
	while (file)
	{
		if (file->d_name[0] != '.')
		{
			filename = ft_strdup(file->d_name);
			if (!filename)
				return (ft_lstclear(files, free), COMMAND_ERROR);
			if (!ft_append(files, filename))
				return (free(filename), ft_lstclear(files, free),
					COMMAND_ERROR);
		}
		file = readdir(dir);
	}
	closedir(dir);
	return (COMMAND_SUCCESS);
}

// OK
static bool	has_only_stars(const char *str)
{
	while (*str)
	{
		if (*str != '*')
			return (false);
		str++;
	}
	return (true);
}

// OK
bool	file_matches_pattern(const char *filename, const char *pattern)
{
	const char	*first_star;

	if (has_only_stars(pattern))
		return (true);
	if (!filename)
		return (false);
	first_star = ft_strchr(pattern, '*');
	if (!first_star)
		return (ft_str_endswith(filename, pattern));
	if (pattern != first_star)
	{
		if (ft_strncmp(filename, pattern, first_star - pattern))
			return (false);
		return (file_matches_pattern(filename + (first_star - pattern),
				first_star));
	}
	while (*pattern == '*')
		pattern++;
	first_star = ft_strchr(pattern, '*');
	if (first_star)
		filename = ft_strstrn(filename, pattern, first_star - pattern);
	return (file_matches_pattern(filename, pattern));
}
