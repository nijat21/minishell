#include "minishell.h"

// CONSTRAINTS -->
// Only one GLOBAL variable to store signal received
//
// FEATURES -->
// Display a prompt when waiting for a command
// Working history
// Search & Launch right executable based on path

/*
	INPUT FOR EXECUTION:
	char *argv[] = {"echo", "-n", "something", NULL}
	
	DISTINCTIONS:
	
	builtin?
	yes
		pipeline?   -> fork
		no pipeline -> parent
	no -> fork + execve	

*/


void echo_func()
{
    char *str;

    if ((str = readline("")) != NULL)
        ft_printf("%s\n", str);
    return;
}

void execute_cmds(int ac, char *av[])
{
	
}


int main(void)
{
    char buf[1024];
    ssize_t by_read;

    while (1)
    {
        write(1, "minishell> ", 11);
        by_read = read(0, buf, sizeof(buf));
        printf("by_read -> %ld\n", by_read);
        if (by_read == 0)
            break;
        if (by_read < 0)
            continue;
        // here: normally parse + execute
        // for now, do nothing
    }
    echo_func();

    return 0;
}
