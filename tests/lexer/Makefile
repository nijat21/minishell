.PHONY: all clean fclean

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBS = -lreadline

SHELL_DIR=../../
LIBFT_DIR=$(SHELL_DIR)/Libft

LEXER_OBJS = $(SHELL_DIR)/lexer.o $(SHELL_DIR)/lexer_utils.o $(SHELL_DIR)/lexer_context.o $(SHELL_DIR)/token_ops.o $(LIBFT_DIR)/libft.a

LEXER_TEST_SRC = lexer.test.c
LEXER_TEST_BIN = lexer_test

all: build_main $(LEXER_TEST_BIN)

build_main:
	make -C $(SHELL_DIR)

$(LEXER_TEST_BIN): $(LEXER_TEST_SRC) $(LEXER_OBJS)
	$(CC) $(CFLAGS) $(LEXER_TEST_SRC) $(LEXER_OBJS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

fclean: clean
	rm -f $(LEXER_TEST_BIN)

clean:
	rm -f $(LEXER_OBJS)

re: fclean all
