/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:13:29 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 14:22:08 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dict	*env_to_dict(char **envp)
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
	return (vars);
}

char	**dict_to_env(t_dict *dict)
{
}
