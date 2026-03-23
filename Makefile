# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/22 23:11:08 by otlacerd          #+#    #+#              #
#    Updated: 2026/03/21 10:22:59 by otlacerd         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
GCC = gcc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address
GFLAGS = -Wall -Wextra -Werror -g3 -O0
RM = rm -rf
OBJ_DIR = build
INCLUDES = -I. -I./includes -I./src/parsing/Libft
LDFLAGS = -lreadline
VALGRIND = valgrind \
			--leak-check=full \
			--track-fds=yes \
			--show-leak-kinds=all \
			--track-origins=yes \
			--suppressions=readline.supp
CHILDREN = --child-silent-after-fork=no \
			--trace-children=yes

LIBFT_MAKE=make -C src/parsing/Libft
LIBFT_LIB=src/parsing/Libft/libft.a

all: $(NAME)

SRCS =	minishell.c \
		src/parsing/lexer/lexer.c \
		src/parsing/lexer/lexer_quotes.c \
		src/parsing/lexer/lexer_utils.c \
		src/parsing/lexer/lexer_context.c \
		src/parsing/lexer/token_ops.c \
		src/parsing/lexer/lexer_list_ops.c \
		src/parsing/lexer/print_token.c \
		src/parsing/pipeline/pipeline.c \
		src/parsing/pipeline/pipeline_utils.c \
		src/parsing/pipeline/pipeline_ops.c \
		src/parsing/pipeline/cmd_list_ops.c \
		src/parsing/pipeline/redir_list_ops.c \
		src/parsing/syntax_check/syntax_check.c \
		src/parsing/syntax_check/syntax_errors.c \
		src/parsing/expand/expand.c \
		src/parsing/print_command/print_command.c \
		src/parsing/parser.c \
		src/execution/core_execution/absolute_path.c \
		src/execution/core_execution/core_execution_utils.c \
		src/execution/core_execution/core_execution.c \
		src/execution/core_execution/fd.c \
		src/execution/core_execution/process.c \
		src/execution/core_execution/pipe.c \
		src/execution/core_execution/redirection_execution.c \
		src/execution/core_execution/redirection.c \
		src/execution/core_execution/heredoc_utils.c \
		src/execution/core_execution/heredoc_execution.c \
		src/execution/builtins/builtins_manager.c \
		src/execution/builtins/env.c \
		src/execution/builtins/env_utils.c \
		src/execution/builtins/exit.c \
		src/execution/builtins/exit_utils.c \
		src/execution/builtins/export_utils.c \
		src/execution/builtins/export.c \
		src/execution/builtins/unset.c \
		src/execution/builtins/echo.c \
 		src/execution/builtins/cd.c \
 		src/execution/builtins/pwd.c \
		src/execution/data/data.c \
		src/execution/data/data_utils.c \
		src/execution/signals/signals.c \
		src/general_utils/allocation.c \
		src/general_utils/allocation_free.c \
		src/general_utils/char.c \
		src/general_utils/conversion.c \
		src/general_utils/error.c \
		src/general_utils/get_next_line.c \
		src/general_utils/string_advanced.c \
		src/general_utils/string_basic.c \


OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) -o $(NAME) $(LDFLAGS)

$(LIBFT_LIB):
	$(LIBFT_MAKE)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:	
	$(RM) $(OBJ_DIR)
	$(LIBFT_MAKE) clean

fclean: clean
	$(RM) $(NAME)
	$(LIBFT_MAKE) fclean

re: fclean all

val: $(NAME)
	$(VALGRIND) ./$(NAME)

valchild: $(NAME)
	$(VALGRIND) $(CHILDREN) ./$(NAME)

re3:
	$(MAKE) fclean
	$(MAKE) CC=$(GCC) CFLAGS="$(GFLAGS)"
	$(VALGRIND) ./$(NAME)

val3:
	$(MAKE) CC=$(GCC) CFLAGS="$(GFLAGS)"
	$(VALGRIND) ./$(NAME)

.PHONY: all clean fclean re val valchild
