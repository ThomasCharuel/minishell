/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:44:55 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/12 12:07:00 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_rand_int(void)
{
	int	res;
	int	fd;

	fd = open("/dev/urandom", O_RDONLY);
	read(fd, &res, sizeof(int));
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
		integers[i++] = ft_itoa(ft_rand_int());
		if (!integers[i])
		{
			while (i--)
				free(integers[i]);
			return (NULL);
		}
	}
	res = ft_strsjoin(integers[0], integers[1], integers[2], integers[3], NULL);
	while (i--)
		free(integers[i]);
	return (res);
}
