/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:37:18 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/01 11:49:40 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strsjoin(char **strs)
{
	size_t	len;
	size_t	i;
	char	*res;

	len = 0;
	i = 0;
	while (strs[i])
		len += ft_strlen(strs[i++]);
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[0] = 0;
	i = 0;
	while (strs[i])
		ft_strcat(res, strs[i++]);
	return (res);
}

char	*prompt_loop(void)
{
	char	*prompt;
	char	*prompt_items[5];
	char	*line;

	prompt_items[0] = getenv("USER");
	prompt_items[1] = ":";
	prompt_items[2] = (char *)malloc(PATH_MAX * sizeof(char));
	if (!prompt_items[2])
		perror("allocation failed, quitting...");
	if (!getcwd(prompt_items[2], PATH_MAX))
		return (free(prompt_items[2]), NULL);
	prompt_items[3] = "$ ";
	prompt_items[4] = NULL;
	prompt = ft_strsjoin(prompt_items);
	free(prompt_items[2]);
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
