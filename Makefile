# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tcharuel <tcharuel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/06 13:51:51 by tcharuel          #+#    #+#              #
#    Updated: 2024/02/13 17:21:14 by tcharuel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

HEADER_DIR = includes
SRC_DIR = src
OBJ_DIR = obj

SRC_FILES = ast.c \
	builtins.c \
	envp.c \
	execution.c \
	main.c \
	parsing.c \
	command.c \
	prompt.c \
	redirection.c \
	signals.c \
	state.c \
	str_list.c \
	tree.c \
	utils.c \
	variable.c \
	heredoc.c \
	pipe.c \
	word.c \
	exit.c \
	env.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wextra -Wall -Werror -g3
RM = rm -f

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_DIR)/$(NAME).h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(HEADER_DIR)/ -c -o $@ $<

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lreadline -lft -o $@

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	make -C $(LIBFT_DIR) clean
	$(RM) -r $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re