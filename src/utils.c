/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:59:02 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/05 15:03:05 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clean_double_list(void **list, void (*destroy)(void *))
{
	void	**temp;

	temp = list;
	if (list)
	{
		while (*temp)
		{
			destroy(*temp);
			temp++;
		}
		free(list);
	}
}
