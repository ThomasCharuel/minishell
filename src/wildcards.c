/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:15:01 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/23 14:40:28 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static void	handle_remove_node(t_list **files, t_list **file,
		t_list **prev_node)
{
	if (*prev_node)
	{
		(*prev_node)->next = (*file)->next;
		ft_lstdelone(*file, free);
		*file = (*prev_node)->next;
	}
	else
	{
		*files = (*file)->next;
		ft_lstdelone(*file, free);
		*file = *files;
	}
}

// OK
static t_command_status	filter_files_based_on_pattern(t_list **files,
		const char *pattern)
{
	t_command_status	status;
	t_list				*file;
	t_list				*prev_node;

	status = get_files_in_current_directory(files);
	if (status)
		return (status);
	file = *files;
	prev_node = NULL;
	while (file)
	{
		if (!file_matches_pattern(file->content, pattern))
		{
			handle_remove_node(files, &file, &prev_node);
			continue ;
		}
		prev_node = file;
		file = file->next;
	}
	return (COMMAND_SUCCESS);
}

// OK
t_command_status	handle_wildcards(char **ptr, t_list *argv)
{
	t_command_status	status;
	t_list				*words;

	if (!ft_strchr(*ptr, '*'))
		return (COMMAND_SUCCESS);
	status = filter_files_based_on_pattern(&words, *ptr);
	if (status)
		return (ft_free_str(ptr), status);
	if (!words)
		return (COMMAND_SUCCESS);
	ft_free_str(ptr);
	while (argv->next)
		argv = argv->next;
	argv->next = words;
	return (COMMAND_SUCCESS);
}
