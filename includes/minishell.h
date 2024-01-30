/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:06:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/01/30 10:45:18 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"

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
}	t_metacharacters;

#endif
