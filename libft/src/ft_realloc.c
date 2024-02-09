/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 23:58:19 by tcharuel          #+#    #+#             */
/*   Updated: 2024/02/10 00:21:45 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void **ptr, size_t len, size_t new_len)
{
	void	*new;

	new = ft_calloc(new_len, sizeof(void *));
	if (!new)
		return (NULL);
	ft_memcpy(new, **ptr, len);
	*ptr = new;
	return (new);
}
