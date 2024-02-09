/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:13:29 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/10 00:13:34 by tcharuel         ###   ########.fr       */
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
	copy = calloc(i + 1, sizeof(char *));
	if (!copy)
		return (perror("minishell"), NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
			return (perror("minishell"), ft_clean_double_list((void **)copy,
					free), NULL);
		i++;
	}
	return (copy);
}

void	envp_cleanup(char ***envp)
{
	if (*envp)
	{
		ft_clean_double_list((void **)*envp, free);
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

t_return_status	envp_set(t_state *state, const char *key, const char *value)
{
	size_t	i;
	size_t	key_len;
	char	*str;

	key_len = ft_strlen(key);
	i = 0;
	while (state->envp[i])
	{
		if (!ft_strncmp(state->envp[i], key, key_len)
			&& state->envp[i][key_len] == '=')
			break ;
		i++;
	}
	str = ft_strsjoin(key, "=", value, NULL);
	if (!str)
		return (ERROR);
	if (state->envp[i])
		free(state->envp[i]);
	else if (!ft_realloc(&state->envp, i, i + 1))
		return (free(str), ERROR);
	state->envp[i] = str;
	return (SUCCESS);
}

t_return_status	envp_delete(char **envp, const char *key)
{
	(void)(envp);
	(void)(key);
	return (SUCCESS);
}
