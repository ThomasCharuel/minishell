/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:25:07 by tcharuel          #+#    #+#             */
/*   Updated: 2023/11/08 10:56:37 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*buffer;
	size_t	bytes;

	bytes = nmemb * size;
	if (size && bytes / size != nmemb)
		return (NULL);
	buffer = malloc(bytes);
	if (!buffer || !nmemb || !size)
		return (buffer);
	ft_bzero(buffer, bytes);
	return (buffer);
}
