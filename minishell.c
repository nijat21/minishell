#include "minishell.h"

// CONSTRAINTS -->
// Only one GLOBAL variable to store signal received
//
// FEATURES -->
// Display a prompt when waiting for a command
// Working history
// Search & Launch right executable based on path

void echo_func()
{
    char *str;

    while ((str = readline("")) != NULL)
        printf("%s", str);
    if (str == NULL)
        printf("\n");
}

int main(void)
{
    char buf[1024];
    ssize_t by_read;

    while (1)
    {
        write(1, "minishell> ", 11);
        echo_func();

        by_read = read(0, buf, sizeof(buf));
        if (by_read == 0)
            break;
        if (by_read < 0)
            continue;
        // here: normally parse + execute
        // for now, do nothing
    }

    return 0;
}
