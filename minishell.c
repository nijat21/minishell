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
#define B_SIZE 1024

int main(void)
{
    char buf[B_SIZE];
    ssize_t by_read;

    int fd = open("no_exist.txt", O_RDONLY);

    while (1)
    {
        write(1, "minishell> ", 11);
        by_read = read(0, buf, sizeof(buf));
        printf("by_read -> %ld buf -> %s\n", by_read, buf);
        if (by_read == 0)
            break;
        if (by_read < 0)
	{
		perror("Read");
		return by_read;
	}
        // here: normally parse + execute
        // for now, do nothing
	
	ft_memcpy(buf,&buf[by_read], BUFFER_SIZE - by_read);
    }

    return 0;
}
