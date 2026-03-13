.PHONY: all clean flcean valgrind re

NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror -g #-fsanitize=address #-O0
VALGRIND = valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp

LIBFT_MAKE=make -C Libft
LIBFT_LIB=Libft/libft.a
OBJ_DIR = build
INCLUDES = -I. -I./includes

SRCS =	minishell.c \
		lexer/lexer.c \
		lexer/lexer_utils.c \
		lexer/lexer_context.c \
		lexer/token_ops.c \
		lexer/lexer_list_ops.c \
		lexer/lex_quotes.c \
		parser.c parser_utils.c syntax_check.c heredoc.c heredoc_utils.c signal.c clean.c\
		pipeline.c pipeline_utils.c pipeline_ops.c expand.c\
		redir_list_ops.c cmd_list_ops.c\
		\
		 print_command.c 
# last line is temporary

OBJS:=$(SRCS:%.c=$(OBJ_DIR)/%.o)

all: ${NAME}

${NAME}: $(LIBFT_LIB) ${OBJS} 
	$(CC) $(CFLAGS) ${OBJS} $(LIBFT_LIB) -lreadline -o ${NAME}

$(LIBFT_LIB):
	$(LIBFT_MAKE)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(LIBFT_MAKE) clean

fclean: clean
	rm -f ${NAME}
	$(LIBFT_MAKE) fclean

valgrind: $(NAME)
	$(VALGRIND) ./$(NAME)

re: fclean all
