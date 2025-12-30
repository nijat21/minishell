.PHONY: all clean flcean re

NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror -g

CFLAGS += -D BUFFER_SIZE=$(BUFFER_SIZE)
BUFFER_SIZE ?= 42
$(info Compiling buffer with BUFFER_SIZE: $(BUFFER_SIZE))

SRCS := ft_isalnum.c ft_itoa.c ft_memchr.c ft_putchar_fd.c \
        ft_strchr.c ft_strlcpy.c ft_strrchr.c ft_isalpha.c \
        ft_memcmp.c ft_putendl_fd.c ft_strdup.c ft_strlen.c \
        ft_strtrim.c ft_atoi.c ft_isascii.c ft_memcpy.c ft_putnbr_fd.c \
        ft_striteri.c ft_strmapi.c ft_substr.c ft_bzero.c ft_isdigit.c \
        ft_memmove.c ft_putstr_fd.c ft_strjoin.c ft_strjoin3.c \
        ft_strncmp.c ft_tolower.c ft_calloc.c ft_isprint.c \
        ft_memset.c ft_split.c ft_strlcat.c ft_strnstr.c ft_toupper.c \
		ft_lstiter.c ft_lstlast.c ft_lstadd_back.c ft_lstadd_front.c ft_lstmap.c \
		ft_lstclear.c ft_lstnew.c ft_lstdelone.c ft_lstsize.c \
		get_next_line.c get_next_line_utils.c \
		ft_printf.c print_string.c print_char.c print_pointer.c print_number.c print_unsigned_number.c print_hex.c


OBJS := $(SRCS:%.c=%.o)

all: ${NAME}

${NAME}: ${OBJS}
	@echo "Linking libft.a"
	ar rcs ${NAME} ${OBJS}

%.o: %.c libft.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -fv ${OBJS} 

fclean: clean
	rm -f ${NAME}

re: fclean all