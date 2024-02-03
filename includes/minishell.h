/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:06:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 12:15:41 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_state
{
	t_dict						*vars;
}								t_state;

extern volatile sig_atomic_t	g_signal_code;

int								echo(int ac, char **av);
char							*pwd(void);
int								cd(int ac, char **av);

char							*prompt_loop(void);
bool							is_whitespace_line(char *line);

void							signal_init(void);

t_state							*state_init(char **envp);
void							state_cleanup(t_state *state);

#endif
