#include "tests.h"

int main()
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


        return 0;
}
