#include "lexer.h"

/*
    Heredoc handler
        - delimeter cannot start with any other meaningful char (|, <, >)
        - echo << > --> zsh: parse error near `>'
        - echo <<   -->zsh: parse error near `\n'

        cat << del>
        heredoc> some
        heredoc> del>
        heredoc> del
        zsh: parse error near `\n'

        cat << -del -> -del is delimeter

        cat << 'del'/"del" -> exp = no, del is delimeter

        cat << del -> exp = yes, ''/"" in prompts are literals

        cat << - del  -> del is the firt argument to heredoc in this case and - is delimiter
        heredoc> some
        heredoc> del
        heredoc> -

        cat << $hello
        heredoc> some
        heredoc> hello
        heredoc> $hello

        cat << 'hello'darling
        heredoc> some
        heredoc> hello
        heredoc> hellodarling

        echo << del some long ass story del
        heredoc> del
        some long ass story del

        cat << del | grep hello
        pipe heredoc> hello world
        pipe heredoc> world
        pipe heredoc> hello again
        pipe heredoc> del
        hello world
        hello again

        cat << del | grep $hello
        pipe heredoc> some
        pipe heredoc> world
        pipe heredoc> other
        pipe heredoc> del
        world

        cat << "some'del'"
        cat << 'some"del"'
*/

/*
    1. Add heredoc token.
        - See if there's '-' in '<<-'. If yes, cut_leading_tabs = true;
    2. Create a delimeter token removing quotes
        - if del in quotes, everything inside heredoc is exp = false
        - else exp = true if there's a valid varname
    3.
*/

/*
    cat << del $hello other del | grep "hello"
    heredoc> some
    heredoc> more
    heredoc> del
*/

// void handle_heredoc(t_lex_ctx *ctx, const char *str, int *i)
// {
//     while (ft_strncmp(prompt, del, ft_strlen(del)) != 0)
//     while (1)
//     {
//         prompt = readline("heredoc> ");
//         if (!prompt)
//         {
//             printf("exit\n");
//             break;
//         }
//         printf("prompt -> %s\n", prompt);
//         (void)ctx;
//         // ctx->tk = lexer(prompt);
//         // if (!ctx->tk)
//         // 	printf("Include proper input\n");
//         // else
//         // 	print_token_list(ctx->tk);
//         free(prompt);
//     }
// }