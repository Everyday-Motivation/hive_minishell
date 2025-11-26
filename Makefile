# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/10 10:26:08 by timurray          #+#    #+#              #
#    Updated: 2025/11/26 17:20:45 by timurray         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell
CC := cc
CPPFLAGS := -Ilibft -Iinclude
CFLAGS := -Wall -Wextra -Werror
VFLAGS := -g
SFLAGS := -ggdb3 -fsanitize=address -fsanitize=leak -fsanitize=undefined
LDLIBS := -lreadline

SOURCES := \
src/minishell.c 

UTIL_DIR := src/utils
UTIL_SRC := \
$(UTIL_DIR)/data_arena.c \
$(UTIL_DIR)/sort_vec_ptr.c \
$(UTIL_DIR)/vec_str_helper.c \
$(UTIL_DIR)/vec_to_arr.c \
$(UTIL_DIR)/error.c

PARS_DIR := src/parsing
PARS_SRC := \
$(PARS_DIR)/env_exp.c \
$(PARS_DIR)/get_env.c \
$(PARS_DIR)/heredoc.c \
$(PARS_DIR)/heredoc_helper.c \
$(PARS_DIR)/parsing.c \
$(PARS_DIR)/parsing_helper.c \
$(PARS_DIR)/prompt.c \
$(PARS_DIR)/tokenizing.c \
$(PARS_DIR)/tokenizing_helper.c 

EXEC_DIR := src/execution
EXEC_SRC := \
$(EXEC_DIR)/execution.c \
$(EXEC_DIR)/execution_helper.c\
$(EXEC_DIR)/find_path.c \
$(EXEC_DIR)/execution_builtin.c \
$(EXEC_DIR)/execution_heredoc.c

BTIN_DIR := src/builtins
BTIN_SRC := \
$(BTIN_DIR)/cd_builtin.c \
$(BTIN_DIR)/echo_builtin.c \
$(BTIN_DIR)/env_builtin.c \
$(BTIN_DIR)/exit_builtin.c \
$(BTIN_DIR)/export_builtin.c \
$(BTIN_DIR)/export_helper.c \
$(BTIN_DIR)/pwd_builtin.c \
$(BTIN_DIR)/unset_builtin.c

SIGN_DIR := src/signal
SIGN_SRC := \
$(SIGN_DIR)/signal.c

SOURCES += ${PARS_SRC}
SOURCES += ${EXEC_SRC}
SOURCES += ${BTIN_SRC}
SOURCES += ${SIGN_SRC}
SOURCES += ${UTIL_SRC}

OBJ_DIR := obj
OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR  := ./libft
LIBFT := $(LIBFT_DIR)/libft.a


debug ?= 0
ifeq (${debug}, 1)
	CFLAGS := ${CFLAGS} ${VFLAGS}
endif
ifeq (${debug}, 2)
	CFLAGS := ${CFLAGS} ${SFLAGS}
endif

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDLIBS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
