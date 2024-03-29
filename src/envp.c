/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:13:29 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/18 16:25:05 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
char	**envp_copy(const char **envp)
{
	size_t	i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = ft_calloc(i + 1, sizeof(char *));
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

const char	*envp_get(t_state *state, const char *key)
{
	size_t	i;
	size_t	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (state->envp[i])
	{
		if (!ft_strncmp(state->envp[i], key, key_len))
		{
			if (!state->envp[i][key_len])
				return (&state->envp[i][key_len]);
			else if (state->envp[i][key_len] == '=')
				return (&state->envp[i][key_len + 1]);
		}
		i++;
	}
	return ("");
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
			&& (!state->envp[i][key_len] || state->envp[i][key_len] == '='))
			break ;
		i++;
	}
	if (value)
		str = ft_strsjoin(key, "=", value, NULL);
	else
		str = ft_strdup(key);
	if (!str)
		return (ERROR);
	if (state->envp[i])
		free(state->envp[i]);
	else if (!ft_realloc((void **)&state->envp, i, i + 1))
		return (free(str), ERROR);
	state->envp[i] = str;
	return (SUCCESS);
}

void	envp_delete(t_state *state, const char *key)
{
	size_t	i;
	size_t	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (state->envp[i])
	{
		if (!ft_strncmp(state->envp[i], key, key_len)
			&& (!state->envp[i][key_len] || state->envp[i][key_len] == '='))
			break ;
		i++;
	}
	if (!state->envp[i])
		return ;
	free(state->envp[i]);
	while (state->envp[i])
	{
		state->envp[i] = state->envp[i + 1];
		i++;
	}
}
