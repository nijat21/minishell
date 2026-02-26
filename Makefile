.PHONY: all clean flcean re

NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror -g

LIBFT_MAKE=make -C Libft
LIBFT_LIB=Libft/libft.a

SRCS:=minishell.c lexer.c lexer_utils.c lexer_context.c token_ops.c lexer_list_ops.c \
		parser.c parser_utils.c syntax_check.c heredoc.c heredoc_utils.c
OBJS:=$(SRCS:%.c=%.o)

all: ${NAME}

${NAME}: $(LIBFT_LIB) ${OBJS} 
	$(CC) $(CFLAGS) ${OBJS} $(LIBFT_LIB) -lreadline -o ${NAME}

$(LIBFT_LIB):
	$(LIBFT_MAKE)

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f ${OBJS} 
	$(LIBFT_MAKE) clean

fclean: clean
	rm -f ${NAME}
	$(LIBFT_MAKE) fclean

re: fclean all
