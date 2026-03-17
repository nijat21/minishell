# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/22 23:11:08 by otlacerd          #+#    #+#              #
#    Updated: 2026/03/17 00:22:09 by otlacerd         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address
LIBFT_DIR = src/parsing/Libft
RM = rm -rf
OBJ_DIR = build
INCLUDES = -I. -I./includes -I./src/parsing/Libft
LDFLAGS = -lreadline -L$(LIBFT_DIR) -lft
VALGRIND = valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp
SRCS =	minishell.c \
		src/execution/data/data.c \
		src/parsing/lexer/lexer.c \
		src/parsing/lexer/lex_quotes.c \
		src/parsing/lexer/lexer_utils.c \
		src/parsing/lexer/lexer_context.c \
		src/parsing/lexer/token_ops.c \
		src/parsing/lexer/lexer_list_ops.c \
		src/parsing/pipeline/pipeline.c \
		src/parsing/pipeline/pipeline_utils.c \
		src/parsing/pipeline/pipeline_ops.c \
		src/parsing/pipeline/cmd_list_ops.c \
		src/parsing/pipeline/redir_list_ops.c \
		src/parsing/syntax_check/syntax_check.c \
		src/parsing/syntax_check/syntax_errors.c \
		src/parsing/heredoc/close_write_fds.c \
		src/parsing/heredoc/heredoc_ops.c \
		src/parsing/heredoc/heredoc_utils.c \
		src/parsing/heredoc/heredoc.c \
		src/parsing/signal/signal.c \
		src/parsing/expand/expand.c \
		src/parsing/parser.c \
		src/execution/core_execution/absolute_path.c \
		src/execution/core_execution/core_execution_utils.c \
		src/execution/core_execution/core_execution.c \
		src/execution/core_execution/fd.c \
		src/execution/core_execution/pid.c \
		src/execution/core_execution/pipe.c \
		src/execution/core_execution/redirection_execution.c \
		src/execution/core_execution/redirection.c \
		src/execution/core_execution/redir_heredoc_utils.c \
		src/general_utils/allocation.c \
		src/general_utils/allocation_free.c \
		src/general_utils/char.c \
		src/general_utils/conversion.c \
		src/general_utils/error.c \
		src/general_utils/get_next_line.c \
		src/general_utils/string_advanced.c \
		src/general_utils/string_basic.c \
		src/execution/built-ins/built-ins_manager.c \
		src/execution/built-ins/env.c \
		src/execution/built-ins/env_utils.c \
		src/execution/built-ins/exit.c \
		src/execution/built-ins/export_utils.c \
		src/execution/built-ins/export.c \
		src/execution/built-ins/unset.c \
		src/execution/built-ins/echo.c \
 		src/execution/built-ins/cd.c \
 		src/execution/built-ins/pwd.c \
		src/execution/signals/signals.c \
		src/execution/data/lst_comand_tools.c

# 		src/execution/data/lst_redir_tools.c \
# 		src/execution/data/create_redir_lst.c \
# 		src/execution/data/create_comand_lst.c \

all: libft $(NAME)

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

$(NAME): $(OBJS) libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libft:
	@$(MAKE) -C $(LIBFT_DIR) libft.a

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

valgrind: $(NAME)
	$(VALGRIND) ./$(NAME)

.PHONY: all clean fclean re valgrind libft
