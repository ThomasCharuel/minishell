/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:37:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/03 18:47:53 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*prompt_loop(void)
{
	char	*pwd;
	char	*prompt;
	char	*line;

	pwd = (char *)malloc(PATH_MAX * sizeof(char));
	if (!pwd)
		perror("allocation failed, quitting...");
	if (!getcwd(pwd, PATH_MAX))
		return (free(pwd), NULL);
	prompt = ft_strsjoin(getenv("USER"), ":", pwd, "$ ", NULL);
	free(pwd);
	if (!prompt)
		return (NULL);
	line = readline(prompt);
	free(prompt);
	return (line);
}

bool	is_whitespace_line(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (false);
		i++;
	}
	return (true);
}
