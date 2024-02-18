/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:45:39 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/18 16:46:48 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static char	*get_minishell_path(const char *executable_path)
{
	char	*path;
	char	*pwd;

	if (executable_path[0] == '/')
		return (ft_strdup(executable_path));
	pwd = get_working_directory();
	if (!pwd)
		return (NULL);
	path = ft_strsjoin(pwd, "/", executable_path, NULL);
	free(pwd);
	return (path);
}

// OK
t_state	*state_init(const char *executable_path, const char **envp)
{
	t_state	*state;

	state = malloc(sizeof(t_state));
	if (!state)
		return (perror("minishell"), NULL);
	state->line = NULL;
	state->last_exit_code = COMMAND_SUCCESS;
	state->envp = envp_copy(envp);
	if (!state->envp)
		return (free(state), NULL);
	state->ast = NULL;
	state->heredocs = NULL;
	state->last_child_pid = 0;
	state->executable_path = get_minishell_path(executable_path);
	if (!state->executable_path)
		return (ft_clean_double_list((void **)state->envp, free), free(state),
			NULL);
	return (state);
}

void	state_cleanup(t_state *state)
{
	if (state)
	{
		ft_free_str(&state->line);
		ft_clean_double_list((void **)state->envp, free);
		node_destroy(&state->ast);
		ft_lstclear(&state->heredocs, &heredoc_destroy);
		ft_free_str(&state->executable_path);
		free(state);
	}
	rl_clear_history();
}
