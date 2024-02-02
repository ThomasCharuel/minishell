/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 11:35:58 by romain            #+#    #+#             */
/*   Updated: 2024/02/02 20:12:07 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// prompt and var utils

/// @brief return the value if exist to a variable indicate by name
/// @param name name of variable
/// @return the adress of the variable or NULL if the variable don't exist
t_variable	*get_var(t_data *data, char *name)
{
	t_variable	*path_var;
	t_lst		*cursor;

	if (!data->vars)
		return (NULL);
	cursor = data->vars->next;
	path_var = NULL;
	while (cursor != data->vars)
	{
		if (!ft_strncmp(((t_variable *)cursor->data)->name, name,
				ft_strlen(name)))
		{
			path_var = cursor->data;
			break ;
		}
		cursor = cursor->next;
	}
	if (!ft_strncmp(((t_variable *)cursor->data)->name, name, ft_strlen(name)))
		path_var = cursor->data;
	if (path_var)
		return (path_var);
	return (NULL);
}

/// @brief create a t_variable
/// @param name name of the variable
/// @param val value of the variable
/// @return a t_variable
t_variable	*write_var(char *name, char *val)
{
	t_variable	*var;

	var = malloc(sizeof(t_variable));
	if (!var)
		malloc_error();
	var->name = malloc(ft_strlen(name) + 1);
	var->value = malloc(ft_strlen(val) + 1);
	if (!var->name || !var->value)
		malloc_error();
	ft_strlcpy(var->name, name, ft_strlen(name) + 1);
	ft_strlcpy(var->value, val, ft_strlen(val) + 1);
	return (var);
}

/// @brief update the data structure with the new value for the name variable
/// @param data data struct
/// @param name name of the variable to update
/// @param value new value of the variable
/// @return	return 0 on success,
/// 1 if the variable doesn't exist on the data struct
int	update_var(t_data *data, char *name, char *value)
{
	t_variable *path_var;

	if (!data->vars)
		return (1);
	path_var = get_var(data, name);
	if (path_var)
	{
		free(path_var->value);
		path_var->value = malloc(ft_strlen(value) + 1);
		if (!path_var->value)
			malloc_error();
		ft_strlcpy(path_var->value, value, ft_strlen(value) + 1);
		return (0);
	}
	return (1);
}