/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <romain@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:06:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/04 19:35:48 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _XOPEN_SOURCE 700
# define IN_FD 0
# define OUT_FD 1

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

typedef enum e_command_status
{
	COMMAND_SUCCESS = 0,
	COMMAND_ERROR = -1,
	COMMAND_NOT_FOUND = 127
}								t_command_status;

typedef enum e_return_status
{
	SUCCESS = 1,
	ERROR = 0,
}								t_return_status;

typedef struct s_command
{
	char						**argv;
	int							status;
	pid_t						pid;
}								t_command;

typedef struct s_state
{
	char						**envp;
	t_command_status			last_exit_code;
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
t_return_status					envp_set(char **envp, const char *key,
									const char *value);
t_return_status					envp_delete(char **envp, const char *key);

t_command						*command_create(const char *command_str);
void							command_destroy(t_command **command);
t_command_status				command_parse(t_state *state,
									t_command *command);

char							*get_command_file(const char *path,
									const char *cmd);
t_command_status				handle_command(t_state *state,
									t_command *command);
t_command_status				handle_path_command(t_command *command);

t_command_status				exec_line(t_state *state,
									const char *command_str);

#endif
