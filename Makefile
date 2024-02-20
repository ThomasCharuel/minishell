# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdupeux <rdupeux@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/06 13:51:51 by tcharuel          #+#    #+#              #
#    Updated: 2024/02/20 13:57:04 by rdupeux          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

HEADER_DIR = includes
SRC_DIR = src
OBJ_DIR = obj

SRC_FILES = ast_execution.c \
	ast_parsing.c \
	ast_parsing_2.c \
	builtins_env.c \
	builtins_export.c \
	builtins.c \
	command_executable.c \
	command_execution.c \
	command_line.c \
	command.c \
	envp.c \
	buitins_exit.c \
	heredoc.c \
	heredocs.c \
	main.c \
	pipe.c \
	prompt.c \
	redirection.c \
	redirections.c \
	signals.c \
	state.c \
	subshell.c \
	tree.c \
	utils.c \
	utils_2.c \
	variable.c \
	wildcards.c \
	word.c \
	buitins_cd.c \
	buitins_echo.c \
	buitins_pwd.c 

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

bonus: all

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

.PHONY: all clean fclean re bonus