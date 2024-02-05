/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:34:15 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/05 15:05:20 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*redirection_create(const char *file, t_redirection_type type)
{
	t_redirection	*redirection;

	redirection = malloc(sizeof(t_redirection));
	if (!redirection)
		return (perror("minishell"), NULL);
	redirection->file = (char *)file;
	redirection->type = type;
	return (redirection);
}

void	redirection_destroy(void *ptr)
{
	t_redirection	*redirection;

	redirection = ptr;
	if (redirection)
	{
		if (redirection->file)
			free(redirection->file);
		free(redirection);
	}
}
