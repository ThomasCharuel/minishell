/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:44:55 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/12 17:46:02 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_rand_int(void)
{
	int	res;
	int	fd;

	fd = open("/dev/urandom", O_RDONLY);
	read(fd, &res, sizeof(int));
	close(fd);
	return (res);
}

char	*ft_rand_uuid(void)
{
	char	*res;
	char	*integers[4];
	size_t	i;

	i = 0;
	while (i < 4)
	{
		integers[i] = ft_ltoa((unsigned int)ft_rand_int(), "0123456789abcdef");
		if (!integers[i])
		{
			while (i--)
				free(integers[i]);
			return (NULL);
		}
		i++;
	}
	res = ft_strsjoin(integers[0], "-", integers[1], "-", integers[2], "-",
			integers[3], NULL);
	while (i--)
		free(integers[i]);
	return (res);
}
