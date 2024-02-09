/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:45:39 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/09 11:20:52 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_state	*state_init(const char **envp)
{
	t_state	*state;

	state = (t_state *)malloc(sizeof(t_state));
	if (!state)
		return (NULL);
	state->line = NULL;
	state->last_exit_code = COMMAND_SUCCESS;
	state->envp = envp_copy(envp);
	if (!state->envp)
		return (free(state), state = NULL, NULL);
	state->ast = NULL;
	state->heredocs = NULL;
	return (state);
}

void	state_cleanup(t_state *state)
{
	if (state)
	{
		ft_free_str(&state->line);
		envp_cleanup(&state->envp);
		node_destroy(&state->ast);
		ft_lstclear(&state->heredocs, &heredoc_destroy);
		free(state);
		state = NULL;
	}
}
