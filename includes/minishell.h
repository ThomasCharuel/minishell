/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:06:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/01/30 19:32:29 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
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

# define ANSI_COLOR_RED "\x1b[31m"
# define ANSI_COLOR_GREEN "\x1b[32m"
# define ANSI_COLOR_YELLOW "\x1b[33m"
# define ANSI_COLOR_BLUE "\x1b[34m"
# define ANSI_COLOR_MAGENTA "\x1b[35m"
# define ANSI_COLOR_CYAN "\x1b[36m"
# define ANSI_COLOR_RESET "\x1b[0m"

int		echo(int ac, char **av);
char	*pwd(void);
int		cd(int ac, char **av);

#endif
