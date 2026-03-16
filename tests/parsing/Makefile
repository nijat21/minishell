.PHONY: all clean fclean

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBS = -lreadline

SHELL_DIR=../../
LIBFT_DIR=../../src/parsing/Libft
BUILD_DIR=$(SHELL_DIR)/build

INCLUDES = -I. \
           -I./includes \
           -I$(SHELL_DIR) \
           -I$(SHELL_DIR)/includes \
           -I$(SHELL_DIR)/src/parsing \
           -I$(SHELL_DIR)/src/parsing/Libft

MAIN_OBJS = $(filter-out $(BUILD_DIR)/minishell.o, \
    		$(shell find $(BUILD_DIR) -name "*.o")) \
			$(LIBFT_DIR)/libft.a

TEST_SRC = integrated.test.c lexer.test.c syntax.test.c pipeline.test.c
TEST_BIN = integrated_test

all: build_main $(TEST_BIN)

build_main:
	make -C $(SHELL_DIR)

$(TEST_BIN): $(TEST_SRC) $(MAIN_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TEST_SRC) $(MAIN_OBJS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TEST_BIN)
	
fclean: clean
	make -C $(SHELL_DIR) fclean

re: fclean all
