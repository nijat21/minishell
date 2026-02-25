#ifndef MINISHELL_H
#define MINISHELL_H

#include "Libft/libft.h"
#include "lexer.h"
#include "parser.h"

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdbool.h>
#include <signal.h>

t_token *lexer(const char *prompt);

typedef struct s_mshell
{
    int exit_code;
} t_mshell;

#endif
