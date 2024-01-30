/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:38:32 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 10:34:17 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*buffer1;
	unsigned char	*buffer2;
	size_t			i;

	if (n == 0)
		return (0);
	buffer1 = (unsigned char *)s1;
	buffer2 = (unsigned char *)s2;
	i = 0;
	while (i < n - 1 && buffer1[i] == buffer2[i])
		i++;
	return (buffer1[i] - buffer2[i]);
}
