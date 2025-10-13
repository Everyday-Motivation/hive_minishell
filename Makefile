# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/10 10:26:08 by timurray          #+#    #+#              #
#    Updated: 2025/10/11 16:56:41 by timurray         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SOURCES = \
src/minishell.c \
src/get_env.c \
src/get_input.c \
src/data_arena.c \
src/signal.c \
src/tokenizing.c src/tokenizing2.c\
src/error.c \
src/prompt.c \
src/execute.c src/find_path.c \
src/parsing.c \
src/vec_str_helper.c \
src/env_exp.c \
src/heredoc.c \
src/sort_vec_ptr.c \
src/execution.c

BUILT_IN_DIR = src/builtins
BUILT_INS = \
$(BUILT_IN_DIR)/cd_builtin.c \
$(BUILT_IN_DIR)/echo_builtin.c \
$(BUILT_IN_DIR)/env_builtin.c \
$(BUILT_IN_DIR)/exit_builtin.c \
$(BUILT_IN_DIR)/export_builtin.c \
$(BUILT_IN_DIR)/export_helper.c \
$(BUILT_IN_DIR)/pwd_builtin.c \
$(BUILT_IN_DIR)/unset_builtin.c

SOURCES += ${BUILT_INS}

OBJ_DIR = obj
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR  = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -I libft/
VFLAGS = -g
SFLAGS = -ggdb3 -fsanitize=address -fsanitize=leak -fsanitize=undefined
LDFLAGS = -lreadline

debug ?= 0
ifeq (${debug}, 1)
	CFLAGS := ${CFLAGS} ${VFLAGS}
endif
ifeq (${debug}, 2)
	CFLAGS := ${CFLAGS} ${SFLAGS}
endif

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
