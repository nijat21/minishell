#include "parser.h"

/* Parser layers ->

    1. Check for syntax errors
        - yes -> return error msg
        - no ->
                exp = true -> expand
                exp = false -> add to the pipeline
    3. Complete pipeline and return

    OUTPUT structure:
    ?

    -----------------------------------------------------------------------------------------
    Grammar:
    Syntax errors (not execution errors)
    -----------------------------------------------------------------------------------------

    Pipe ->
    | ls
    bash: syntax error near unexpected token `|'
    ls |
    > ^C
    ls | | wc
    bash: syntax error near unexpected token `|'
    ls ||| wc
    bash: syntax error near unexpected token `|'
    ls || wc
    Makefile  lexer.test.c  lexer_test  lexer_test.dSYM


    Redirs ->
    ls >
    bash: syntax error near unexpected token `newline'
    ls >>
    bash: syntax error near unexpected token `newline'
    ls <<
    bash: syntax error near unexpected token `newline'
    ls <
    bash: syntax error near unexpected token `newline'


    Redirs + Pipe ->
    ls >|
    bash: syntax error near unexpected token `newline'


    Redirs + Redirs
    ls > > file
    bash: syntax error near unexpected token `>'
    ls < << file
    bash: syntax error near unexpected token `<<'


    Heredoc ->
    ls <<
    bash: syntax error near unexpected token `newline'
    ls << | wc


    NULL ->
    bash: syntax error near unexpected token `|'
    |
    bash: syntax error near unexpected token `|'
    ||
    bash: syntax error near unexpected token `||'
    |||
    bash: syntax error near unexpected token `||'

  */

/*
      Solution:

          1. Token checker (PIPE + PIPE, WORD + PIPE combo ect.)
          2. Seg checker (ls > > file etc.)

*/


void *parse_tokens(t_token *tk)
{
    t_token *temp;
    int pipe;
    int i;

    i = -1;
    pipe = -1;
    temp = tk;
    while (temp)
    {
        ++i;
        if (temp->type == PIPE)
        {
            if (i == 0)
            {
                print_syntax_error(temp);
                return NULL;
            }
            else if (i > 0 && pipe == -1)
                pipe = i;
            else if (pipe != -1 && i == pipe + 1)
            {
                print_syntax_error(temp);
                return NULL;
            }
        }
        temp = temp->next;
    }
    return NULL;
}
