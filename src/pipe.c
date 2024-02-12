/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:21:50 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/12 13:25:11 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe	*pipe_create(void)
{
	t_pipe	*my_pipe;

	my_pipe = malloc(sizeof(t_pipe));
	if (!my_pipe)
		return (NULL);
	if (pipe(my_pipe->fd))
		return (free(my_pipe), NULL);
	return (my_pipe);
}
