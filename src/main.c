/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:08:12 by tcharuel          #+#    #+#             */
/*   Updated: 2024/01/30 20:25:06 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_prompt(void)
{
	char	*prompt;
	char	*res;
	char	*wd;
	int		i;

	i = 0;
	prompt = malloc(1024);
	res = prompt;
	wd = pwd();
	i = 0;
	while (i <= 18)
		*prompt++ = ANSI_COLOR_RED "Minishell" ANSI_COLOR_RESET "-"[i++];
	i = 0;
	while (i < 5)
		*prompt++ = ANSI_COLOR_GREEN[i++];
	i = 0;
	while (wd[i])
		*prompt++ = wd[i++];
	i = 0;
	while (i < 4)
		*prompt++ = ANSI_COLOR_RESET[i++];
	*prompt++ = '\n';
	*prompt++ = '\0';
	return (res);
}

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

int	main(int argc, char **argv)
{
	char	*line;

	(void)argc;
	(void)argv;
	line = prompt_loop();
	while (line)
	{
		if (!is_whitespace_line(line))
		{
			add_history(line);
			ft_printf("%s\n", line);
		}
		free(line);
		line = prompt_loop();
	}
	return (0);
}
