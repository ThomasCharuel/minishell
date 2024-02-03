/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:13:29 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 16:06:37 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**envp_copy(const char **envp)
{
	size_t	i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = (char **)malloc((i + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i-- > 1)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	envp_cleanup(char ***envp)
{
	size_t	i;

	if (*envp)
	{
		i = 0;
		while (*envp[i])
			free(*envp[i++]);
		free(*envp);
		*envp = NULL;
	}
}

const char	*envp_get(const char **envp, const char *key)
{
	size_t	i;
	size_t	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, key_len) && envp[i][key_len] == '=')
			return (&envp[i][key_len + 1]);
		i++;
	}
	return (NULL);
}

t_return_code	envp_set(char **envp, const char *key, const char *value)
{
	(void)(envp);
	(void)(key);
	(void)(value);
	return (SUCCESS);
}

t_return_code	envp_delete(char **envp, const char *key)
{
	(void)(envp);
	(void)(key);
	return (SUCCESS);
}
