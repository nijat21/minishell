#include "minishell.h"


/* 
	USE CASES:
	echo 			-> $
	echo -n			-> 
	echo ""			-> $
	echo -n ""		-> 
	echo something		-> something$
	echo -n something 	-> something	

	INPUT TO FUNCTION:
	cmd			-> echo
	flag			-> -n
	string			-> something

	char *argv[] = {"echo", "-n", "something", NULL}
	
	Why to use fork/pipe at all?
*/

// tracer and history generation
void echo()
{
    char *str;

    while ((str = readline("")) != NULL)
        printf("%s", str);
    if (str == NULL)
        printf("\n");
}
