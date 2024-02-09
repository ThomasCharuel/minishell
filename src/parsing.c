/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 14:41:51 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/09 10:25:10 by tcharuel         ###   ########.fr       */
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

	command_str = command->argv->content;
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
				free(command->argv->content),
				command->argv->content = command_path, COMMAND_SUCCESS);
		free(command_path);
		i++;
	}
	write(STDERR_FILENO, command_str, ft_strlen(command_str));
	write(STDERR_FILENO, ": command not found\n", 21);
	return (ft_clean_double_list((void **)paths, free), COMMAND_NOT_FOUND);
}

t_command_status	handle_path_command(t_command *command)
{
	char	*command_str;

	command_str = command->argv->content;
	if (access(command_str, R_OK) || access(command_str, X_OK))
	{
		perror(command_str);
		return (COMMAND_NOT_FOUND);
	}
	return (COMMAND_SUCCESS);
}

t_command_status	handle_word_interpretation(t_state *state, char **str)
{
	t_list		*words;
	char		*word;
	bool		is_double_quote;
	const char	*cursor;
	const char	*new_cursor;

	is_double_quote = false;
	words = NULL;
	cursor = *str;
	new_cursor = cursor;
	while (*cursor)
	{
		if (is_double_quote)
			new_cursor = ft_strchrs(new_cursor, "\"$");
		else
			new_cursor = ft_strchrs(new_cursor, "\"\'$");
		if (!new_cursor)
		{
			word = ft_strdup(cursor);
			if (!word)
				return (ft_lstclear(&words, free), COMMAND_ERROR);
			if (!str_list_append(&words, word))
				return (free(word), ft_lstclear(&words, free), COMMAND_ERROR);
			break ;
		}
		word = ft_strndup(cursor, new_cursor - cursor);
		if (!word)
			return (ft_lstclear(&words, free), COMMAND_ERROR);
		if (!str_list_append(&words, word))
			return (free(word), ft_lstclear(&words, free), COMMAND_ERROR);
		cursor = new_cursor;
		if (*new_cursor == '\'')
		{
			new_cursor = ft_strchr(++cursor, '\'');
			if (!new_cursor)
				return (ft_lstclear(&words, free), COMMAND_PARSING_ERROR);
			word = ft_strndup(cursor, new_cursor - cursor);
			if (!word)
				return (ft_lstclear(&words, free), COMMAND_ERROR);
			if (!str_list_append(&words, word))
				return (free(word), ft_lstclear(&words, free), COMMAND_ERROR);
			cursor = ++new_cursor;
		}
		else if (*new_cursor == '\"')
		{
			if (is_double_quote)
			{
				word = ft_strndup(cursor, new_cursor - cursor);
				if (!word)
					return (ft_lstclear(&words, free), COMMAND_ERROR);
				if (!str_list_append(&words, word))
					return (free(word), ft_lstclear(&words, free),
						COMMAND_ERROR);
				is_double_quote = false;
			}
			else
				is_double_quote = true;
			cursor = ++new_cursor;
		}
		else if (*new_cursor++ == '$')
		{
			word = get_var_value(state, &new_cursor);
			if (!word)
				return (ft_lstclear(&words, free), COMMAND_ERROR);
			if (!str_list_append(&words, word))
				return (free(word), ft_lstclear(&words, free), COMMAND_ERROR);
			cursor = new_cursor;
		}
	}
	free(*str);
	*str = ft_strsjoin_from_list(words);
	ft_lstclear(&words, free);
	if (!*str)
		return (COMMAND_ERROR);
	return (COMMAND_SUCCESS);
}

t_command_status	get_next_word(const char **ptr, char **word)
{
	const char	*cursor;
	const char	*new_cursor;

	cursor = *ptr;
	new_cursor = cursor;
	while (*cursor)
	{
		new_cursor = ft_strchrs(new_cursor, "><\"\' ");
		if (!new_cursor)
		{
			*word = strndup(cursor, ft_strlen(cursor));
			if (!*word)
				return (COMMAND_ERROR);
			cursor += ft_strlen(cursor);
			break ;
		}
		else if (ft_is_char_in_set(new_cursor[0], "<> "))
		{
			*word = strndup(cursor, new_cursor - cursor);
			if (!*word)
				return (COMMAND_ERROR);
			cursor = new_cursor;
			break ;
		}
		else
		{
			new_cursor = ft_strchr(&new_cursor[1], new_cursor[0]);
			if (!new_cursor)
				return (COMMAND_PARSING_ERROR);
			new_cursor++;
		}
	}
	*ptr = cursor;
	return (COMMAND_SUCCESS);
}
