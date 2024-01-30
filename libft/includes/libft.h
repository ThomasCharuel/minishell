/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:01:58 by tcharuel          #+#    #+#             */
/*   Updated: 2024/01/09 11:28:20 by tcharuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

# define CONVERSION_CHAR_CHARACTER 'c'
# define CONVERSION_CHAR_STRING 's'
# define CONVERSION_CHAR_POINTER 'p'
# define CONVERSION_CHAR_DECIMAL 'd'
# define CONVERSION_CHAR_INTEGER 'i'
# define CONVERSION_CHAR_UNSIGNED_DECIMAL 'u'
# define CONVERSION_CHAR_HEX_LOWERCASE 'x'
# define CONVERSION_CHAR_HEX_UPPERCASE 'X'
# define CONVERSION_CHAR_PERCENT '%'

# define BASE_DECIMAL "0123456789"
# define BASE_HEX_LOWERCASE "0123456789abcdef"
# define BASE_HEX_UPPERCASE "0123456789ABCDEF"

# define BUFFER_SIZE 40

typedef struct s_substring
{
	char			*format;
	size_t			format_length;
	char			*result;
	size_t			result_length;
}					t_substring;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_isspace(const char c);
size_t				ft_strlen(const char *s);
void				*ft_memset(void *s, int c, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
char				*ft_strcat(char *dest, const char *src);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
int					ft_toupper(int c);
int					ft_tolower(int c);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
int					ft_atoi(const char *nptr);
long				ft_atol(const char *nptr);
void				*ft_calloc(size_t nmemb, size_t size);
char				*ft_strdup(const char *s);
char				*ft_strndup(const char *s, size_t n);

char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_lutoa(unsigned long n, char *base);
char				*ft_ltoa(long l, char *base);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

int					ft_printf(const char *format, ...);

int					is_conversion_identifier(const char c);
t_list				*parse_format(const char *format, va_list args);
t_substring			*parse_substring(const char **format, va_list args);

t_substring			*create_substring(char *format, size_t format_length);
void				free_substring(t_substring *substring);
void				put_substring_result(t_substring *substring);
int					get_len_substrings(t_list *lst);

int					interpret_substring(t_substring *substring, va_list args);

char				*get_string_format(va_list args);
char				*get_char_format(va_list args);
char				*get_percent_format(void);
char				*get_pointer_format(va_list args);

int					ft_min(int count, ...);
int					ft_max(int count, ...);

char				*get_next_line(int fd);

size_t				ft_strlen_delimiter(const char *s, char c);
char				*ft_strdup_lf(const char *src);
int					ft_str_has_lf(char const *s);
void				ft_strcpy(char *dst, const char *src);

void				ft_free_strs(char **strs);

#endif
