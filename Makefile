# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/10 10:26:08 by timurray          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2025/09/24 12:48:55 by jaeklee          ###   ########.fr        #
=======
#    Updated: 2025/09/24 12:59:42 by timurray         ###   ########.fr        #
>>>>>>> dev
#                                                                              #
# **************************************************************************** #

NAME = minishell

SOURCES = \
src/minishell.c \
src/get_env.c \
src/get_input.c \
src/data_arena.c \
src/signal.c \
src/tokenizing.c src/lexer.c \
src/signal.c \
src/tokenizing.c \
src/error.c src/prompt.c \

$(BUILT_INS)

BUILT_IN_DIR = src/builtins
BUILT_INS = \
$(BUILT_IN_DIR)/cd_builtin.c $(BUILT_IN_DIR)/echo_builtin.c \
$(BUILT_IN_DIR)/env_builtin.c $(BUILT_IN_DIR)/exit_builtin.c \
$(BUILT_IN_DIR)/export_builtin.c s$(BUILT_IN_DIR)/pwd_builtin.c \
$(BUILT_IN_DIR)/unset_builtin.c \

OBJECTS = $(SOURCES:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I libft/
VFLAGS = -g
SFLAGS = -ggdb3 -fsanitize=address -fsanitize=leak -fsanitize=undefined
LDFLAGS = -lreadline

LIBFT_DIR  = ./libft
LIBFT = $(LIBFT_DIR)/libft.a 

debug ?= 0

ifeq (${debug}, 1)
	CFLAGS := ${CFLAGS} ${VFLAGS}
endif
ifeq (${debug}, 2)
	CFLAGS := ${CFLAGS} ${SFLAGS}
endif

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJECTS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
