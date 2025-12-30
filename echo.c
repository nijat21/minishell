#include "minishell.h"

// tracer and history generation
void echo()
{
    char *str;

    while ((str = readline("")) != NULL)
        printf("%s", str);
    if (str == NULL)
        printf("\n");
}
