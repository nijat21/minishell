#include "tests.h"

int	g_signal;

int	main(void)
{
	// lexer for proper tokenization
	printf(BOLD MAGENTA "LEXER TESTS\n" RESET);
	printf(BOLD MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
	test_lexer();
	printf(BOLD MAGENTA "---------------------------------------------------------------------------------------------\n\n");
	// syntax_checker uses lexer for tokenization
	printf(BOLD MAGENTA "SYNTAX TESTS\n" RESET);
	printf(BOLD MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
	test_syntax();
	printf(BOLD MAGENTA "---------------------------------------------------------------------------------------------\n\n");
	//
	printf(BOLD MAGENTA "PIPELINE TESTS\n" RESET);
	printf(BOLD MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
	test_pipeline();
	printf(BOLD MAGENTA "---------------------------------------------------------------------------------------------\n\n" RESET);
	return (0);
}
