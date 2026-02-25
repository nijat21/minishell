#ifndef TEST_H
#define TEST_H

#include "../lexer.h"
#include "../parser.h"
#include <string.h>
#include "../../Libft/libft.h"

#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

t_token *lexer(const char *prompt);

// Lexer
void test_lexer();
void test_syntax();

#endif