/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:34:20 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/07 14:50:02 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_return_status	str_list_append(t_list **word_list, char *str)
{
	if (!ft_strlen(str))
		return (SUCCESS);
	if (!ft_append(word_list, str))
		return (ERROR);
	return (SUCCESS);
}
