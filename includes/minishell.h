/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:06:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 20:58:44 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _XOPEN_SOURCE 700

# include "../libft/includes/libft.h"
# include <dirent.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_exit_code
{
	COMMAND_SUCCESS = 0,
	COMMAND_NOT_FOUND = 127
}								t_exit_code;

typedef struct s_command
{
	char						**argv;
}								t_command;

typedef struct s_state
{
	char						**envp;
	t_exit_code					last_exit_code;
}								t_state;

extern volatile sig_atomic_t	g_signal_code;

char							*prompt_loop(void);
bool							is_whitespace_line(char *line);

void							signal_init(void);

t_state							*state_init(const char **envp);
void							state_cleanup(t_state *state);

char							**envp_copy(const char **envp);
void							envp_cleanup(char ***envp);
const char						*envp_get(const char **envp, const char *key);
t_return_code					envp_set(char **envp, const char *key,
									const char *value);
t_return_code					envp_delete(char **envp, const char *key);

t_command						*command_create(const char *command_str);
void							command_destroy(t_command **command);

t_return_code					ft_exec(t_state *state, const char *command);

#endif
