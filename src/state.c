/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:45:39 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/08 18:51:51 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_state	*state_init(const char **envp)
{
	t_state	*state;

	state = (t_state *)malloc(sizeof(t_state));
	if (!state)
		return (NULL);
	state->last_exit_code = COMMAND_SUCCESS;
	state->envp = envp_copy(envp);
	if (!state->envp)
		return (free(state), state = NULL, NULL);
	state->pipes = NULL;
	state->commands = NULL;
	state->heredocs = NULL;
	return (state);
}

void	state_cleanup(t_state *state)
{
	if (state)
	{
		envp_cleanup(&state->envp);
		ast_cleanup(state);
		ft_lstclear(&state->heredocs, &heredoc_destroy);
		free(state);
		state = NULL;
	}
}
