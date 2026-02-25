.PHONY: all clean fclean

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBS = -lreadline

LIBFT_DIR=$(SHELL_DIR)/Libft
SHELL_DIR=../

TEST_OBJS = $(filter-out $(SHELL_DIR)/minishell.o, $(wildcard $(SHELL_DIR)/*.o)) $(LIBFT_DIR)/libft.a

TEST_SRC = integrated.test.c lexer.test.c syntax.test.c
TEST_BIN = integrated_test

all: build_main $(TEST_BIN)

build_main:
	make -C $(SHELL_DIR)

$(TEST_BIN): $(TEST_SRC) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_SRC) $(TEST_OBJS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

fclean: clean
	rm -f $(TEST_BIN)
	make -C $(SHELL_DIR) fclean
	

clean:
	rm -f $(TEST_OBJS)

re: fclean all
