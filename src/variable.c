/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:38:57 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/07 13:35:50 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(t_state *state, const char **ptr)
{
	const char	*value;
	const char	*cursor;
	char		*var_str;

	cursor = *ptr;
	if (!ft_isalpha(*cursor) && *cursor != '_')
		return (calloc(1, 1));
	while (ft_isalnum(*cursor) || *cursor == '_')
		cursor++;
	var_str = ft_strndup(*ptr, cursor - *ptr);
	if (!var_str)
		return (NULL);
	*ptr = cursor;
	value = envp_get((const char **)state->envp, var_str);
	free(var_str);
	if (!value)
		return (calloc(1, 1));
	return (ft_strdup(value));
}
