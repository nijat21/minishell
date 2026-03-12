#include "includes/parser.h"

// segments of token 1. user, exp=false, 2. $VAR, exp=true -> only expand seg 2
char *expand_var(char *str)
{
    // IF EXIT_STATUS IS VARIABLE IN STRUCT AND NOT IN ENV
    // TREAT $? BASED ON THAT
    return str;
}

// expands redir arg user$VAR'more' -> userJOE'more'
char *expand_redir_arg(const char *str)
{
    // IF EXIT_STATUS IS VARIABLE IN STRUCT AND NOT IN ENV
    // TREAT $? BASED ON THAT
    /*
    char *var;

    var = ft_strchr(str, '$'); // user$VAR -> str + 4
    */
    return (char *)str;
}