/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:06:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/12 18:23:31 by tcharuel         ###   ########.fr       */
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
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define COMMAND_SUCCESS 0
# define COMMAND_ERROR -1
# define COMMAND_PARSING_ERROR 2
# define COMMAND_NOT_FOUND 127
# define COMMAND_SIGINT 130

typedef int						t_command_status;

typedef enum e_return_status
{
	SUCCESS = 1,
	ERROR = 0,
}								t_return_status;

typedef enum e_redirection_type
{
	WRITE = 1,
	APPEND = 2,
	READ = 3,
}								t_redirection_type;

typedef struct s_redirection
{
	char						*file;
	t_redirection_type			type;
}								t_redirection;

typedef struct s_command
{
	char						*command_str;
	t_list						*argv;
	t_list						*redirections;
	int							status;
	t_command_status			exit_code;
}								t_command;

typedef struct s_heredoc
{
	char						*file;
	char						*eof;
}								t_heredoc;

typedef struct s_pipe
{
	int							fd[2];
}								t_pipe;

typedef enum e_node_type
{
	AND,
	OR,
	PIPE,
	COMMAND
}								t_node_type;

typedef struct s_node
{
	t_node_type					type;
	void						*content;
	struct s_node				*daddy;
	struct s_node				*left;
	struct s_node				*right;
	int							read_fd;
	int							write_fd;
}								t_node;

typedef struct s_state
{
	const char					*executable_path;
	char						**envp;
	char						*line;
	t_command_status			last_exit_code;
	t_node						*ast;
	t_list						*heredocs;
	pid_t						last_child_pid;
}								t_state;

extern volatile sig_atomic_t	g_signal_code;

t_command_status				repl(t_state *state);

void							signal_init(void);

t_state							*state_init(const char *executable_path,
									const char **envp);
void							state_cleanup(t_state *state);

char							**envp_copy(const char **envp);
const char						*envp_get(t_state *state, const char *key);
t_return_status					envp_set(t_state *state, const char *key,
									const char *value);
void							envp_delete(t_state *state, const char *key);

t_command						*command_create(const char *command_str);
void							command_destroy(void *ptr);
t_command_status				command_parse(t_state *state,
									t_command *command);

void							heredoc_destroy(void *ptr);

t_redirection					*redirection_create(const char *file,
									t_redirection_type type);
void							redirection_destroy(void *ptr);
t_command_status				handle_redirection(t_state *state,
									const char **cmd, t_command *command);

char							*get_command_file(const char *path,
									const char *cmd);
t_command_status				handle_command(t_state *state,
									t_command *command);
t_command_status				handle_path_command(t_command *command);
t_command_status				get_next_word(const char **ptr, char **word);

t_command_status				line_exec(t_state *state, const char *line);

void							ft_clean_double_list(void **list,
									void (*destroy)(void *));
void							ft_close_fd(int fd);

char							*get_var_value(t_state *state,
									const char **ptr);
char							*ft_strsjoin_from_list(t_list *list);
void							ft_free(void **ptr);
void							ft_free_str(char **str);

t_return_status					str_list_append(t_list **word_list,
									const char *str);

t_command_status				handle_word_interpretation(t_state *state,
									char **str);
t_command_status				handle_heredocs(t_state *state,
									const char *line);

t_node							*node_create(t_node_type type, void *content);
void							node_destroy(t_node **node);
void							tree_dfs(t_state *state, t_node *node,
									void (*f)(t_state *, t_node *));
void							display_node(t_state *state, t_node *node);
t_command_status				ast_execute(t_state *state, t_node *node);

t_return_status					command_exec(t_state *state, t_node *node);

t_pipe							*pipe_create(void);

t_command_status				get_next_word_new(const char **cursor,
									char **res, const char *charset,
									bool delim);
t_command_status				get_next_heredoc_eof(const char **cursor,
									char **eof);
t_command_status				get_next_expression(const char **ptr,
									char **res);
t_command_status				get_next_command(const char **ptr, char **res);
t_command_status				get_next_parenthesis_expression(const char **ptr,
									char **res);

#endif
