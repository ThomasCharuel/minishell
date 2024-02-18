/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:45:39 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/18 17:55:15 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// OK
static char	*get_minishell_path(const char *executable_path)
{
	char	*path;
	char	*pwd;

	if (executable_path[0] == '/')
		path = ft_strdup(executable_path);
	else
	{
		pwd = get_working_directory();
		if (!pwd)
			return (NULL);
		path = ft_strsjoin(pwd, "/", executable_path, NULL);
		free(pwd);
	}
	if (!path)
		return (perror("minishell"), NULL);
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

// OK
void	state_cleanup(t_state *state)
{
	if (state)
	{
		ft_free_str(&state->executable_path);
		ft_clean_double_list((void **)state->envp, free);
		ft_free_str(&state->line);
		node_destroy(&state->ast);
		ft_lstclear(&state->heredocs, &heredoc_destroy);
		free(state);
	}
	rl_clear_history();
}
