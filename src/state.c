/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:45:39 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 12:42:45 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dict	*init_vars_dict(char **envp)
{
	t_dict	*vars;
	size_t	i;
	char	**env_item;

	vars = ft_dict_init();
	if (!vars)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env_item = ft_split(envp[i], '=');
		if (!env_item)
			return (ft_dict_destroy(&vars), NULL);
		if (!ft_dict_set(vars, env_item[0], env_item[1]))
			return (ft_dict_destroy(&vars), free(env_item[0]),
				free(env_item[1]), free(env_item), NULL);
		free(env_item);
		i++;
	}
	ft_printf("PATH: %s\n", ft_dict_get(vars, "PATH"));
	return (vars);
}

t_state	*state_init(char **envp)
{
	t_state	*state;

	state = (t_state *)malloc(sizeof(t_state));
	if (!state)
		return (NULL);
	state->vars = init_vars_dict(envp);
	if (!state->vars)
		return (free(state), state = NULL, NULL);
	return (state);
}

void	state_cleanup(t_state *state)
{
	if (state)
	{
		if (state->vars)
			ft_dict_destroy(&state->vars);
		free(state);
		state = NULL;
	}
}
