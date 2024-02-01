/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:06:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/01 11:40:09 by tcharuel         ###   ########.fr       */
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

typedef enum e_metacharacters
{
	META_SINGLE_QUOTE = '\'',
	META_DOUBLE_QUOTE = '"',
	META_DOLLAR_SIGN = '$',
	META_QUESTION_MARK = '?',
	META_LESS_THAN = '<',
	META_GREATER_THAN = '>',
	META_PIPE = '|',
	META_AMPERSAND = '&',
	META_LEFT_PARENTHESIS = '(',
	META_RIGHT_PARENTHESIS = ')',
	META_STAR = '*'
}		t_metacharacters;

int		echo(int ac, char **av);
char	*pwd(void);
int		cd(int ac, char **av);

char	*prompt_loop(void);
bool	is_whitespace_line(char *line);

#endif
