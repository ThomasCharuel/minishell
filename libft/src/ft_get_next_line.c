/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:47:08 by tcharuel          #+#    #+#             */
/*   Updated: 2024/01/09 22:09:43 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	cleanup(char *line, char *buffer)
{
	char	*eol_cursor;

	ft_bzero(buffer, BUFFER_SIZE);
	if (line)
	{
		eol_cursor = ft_strchr(line, '\n');
		if (eol_cursor)
		{
			ft_strlcpy(buffer, ++eol_cursor, BUFFER_SIZE);
			eol_cursor[0] = '\0';
		}
	}
}

char	*ft_strljoin_and_free(char *s1, char *s2, size_t len)
{
	char	*res;
	size_t	s1_len;

	s1_len = ft_strlen(s1);
	res = (char *)malloc((s1_len + len + 1) * sizeof(char));
	if (!res)
	{
		free(s1);
		return (NULL);
	}
	ft_strcpy(res, s1);
	ft_strlcat(res, s2, s1_len + len + 1);
	free(s1);
	return (res);
}

/**
 * @brief Returns a line read from a
 * file descriptor.
 *
 * The returned line should include the terminating \n character,
 * except if the end of file was reached and does not end with a \n character.
 *
 * @param fd: The file descriptor to read from.
 *
 * @return Read line: correct behavior.
 * @retval NULL there is nothing else to read, or an error occurred
 */
char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE] = {0};
	char		*line;
	size_t		bytes_read;

	if (BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (NULL);
	line = ft_strdup(buffer);
	if (!line)
		return (NULL);
	bytes_read = BUFFER_SIZE;
	while (!ft_strchr(line, '\n') && bytes_read == BUFFER_SIZE)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		line = ft_strljoin_and_free(line, buffer, bytes_read);
		if (!line)
			return (NULL);
	}
	if (ft_strlen(line) == 0)
	{
		free(line);
		line = NULL;
	}
	cleanup(line, buffer);
	return (line);
}
