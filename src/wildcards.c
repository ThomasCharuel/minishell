/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:15:01 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/14 13:39:01 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*get_files_in_current_directory(void)
{
	t_list			*files;
	DIR				*dir;
	struct dirent	*file;
	char			*pwd;
	char			*filename;

	files = NULL;
	pwd = get_working_directory();
	if (!pwd)
		return (NULL);
	dir = opendir(pwd);
	file = readdir(dir);
	while (file)
	{
		if (file->d_name[0] != '.')
		{
			filename = ft_strdup(file->d_name);
			if (!filename)
				return (ft_lstclear(&files, free), NULL);
			ft_append(&files, filename);
		}
		file = readdir(dir);
	}
	if (closedir(dir) == -1)
		perror("closedir");
	return (files);
}

bool	has_only_stars(const char *str)
{
	while (*str)
	{
		if (*str != '*')
			return (false);
		str++;
	}
	return (true);
}

bool	file_matches_pattern(const char *filename, const char *pattern)
{
	const char	*first_star;

	if (has_only_stars(pattern))
		return (true);
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
	return (file_matches_pattern(filename, pattern));
}

t_list	*filter_files_based_on_pattern(const char *pattern)
{
	t_list	*files;
	t_list	*file;
	t_list	*prev_node;

	files = get_files_in_current_directory();
	file = files;
	prev_node = NULL;
	while (file)
	{
		if (!file_matches_pattern(file->content, pattern))
		{
			if (prev_node)
			{
				prev_node->next = file->next;
				free(file->content);
				free(file);
				file = prev_node->next;
			}
			else
			{
				files = file->next;
				free(file->content);
				free(file);
				file = files;
			}
		}
		else
		{
			prev_node = file;
			file = file->next;
		}
	}
	return (files);
}
