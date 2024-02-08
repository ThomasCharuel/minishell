/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:23:44 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/08 19:15:47 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac == 1)
		write(1, "\n", 1);
	if (ac >= 2 && !ft_strncmp("-n", av[1], 2))
	{
		i++;
		while (++i < ac)
		{
			write(1, av[i], ft_strlen(av[i]));
			if (i + 1 < ac)
				write(1, " ", 1);
		}
		return (0);
	}
	if (ac >= 2)
		while (++i < ac)
		{
			write(1, av[i], ft_strlen(av[i]));
			if (i + 1 < ac)
				write(1, " ", 1);
		}
	write(1, "\n", 1);
	return (0);
}

int	cd(int ac, char **av)
{
	char	*home;

	home = getenv("HOME"); // To refacto
	if (ac == 1)
		return (chdir(home));
	if (ac > 2)
	{
		write(2, "too many arguments", 19);
	}
	else if (chdir(av[1]))
		perror(av[1]);
	return (1);
}

char	*pwd(void)
{
	char	*wd;

	wd = (char *)malloc(PATH_MAX * sizeof(char));
	if (!wd)
		perror("allocation failed, quitting...");
	getcwd(wd, PATH_MAX);
	return (wd);
}
