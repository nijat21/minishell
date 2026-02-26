#include "parser.h"

/*
    Heredoc handler
        - delimeter cannot start with any other meaningful char (|, <, >)
        - echo << > --> zsh: parse error near `>'
        - echo <<   -->zsh: parse error near `\n'

        cat << del>
        > some
        > del>
        > del
        zsh: parse error near `\n'

        cat << -del -> -del is delimeter

        cat << 'del'/"del" -> exp = no, del is delimeter

        cat << del -> exp = yes, ''/"" in prompts are literals

        cat << - del  -> del is the firt argument to heredoc in this case and - is delimiter
        > some
        > del
        > -

        cat << $hello
        > some
        > hello
        > $hello

        cat << 'hello'darling
        > some
        > hello
        > hellodarling

        cat << $user'hello'"  "
        > some
        > $userhello
        > $userhello
        > $userhello
        some
        $userhello
        $userhello

        echo << del some long ass story del
        > del
        some long ass story del

        cat << del | grep hello
        > hello world
        > world
        > hello again
        > del
        hello world
        hello again

        cat << del | grep $hello
        > some
        > world
        > other
        > del
        world

        cat << "some'del'"
        cat << 'some"del"'

        SOME OTHER EDGE CASES

        cat << del -ls
        > some
        > del
        some

        cat << del -l
        > some
        > del
        some

        cat << del some
        > other
        > del
        cat: some: No such file or directory

        hello=world
        cat << del $hello
        > some
        > del
        cat: world: No such file or directory

        echo world > input.txt
        cat << del input.txt
        > some
        > del
        world

        cat input.txt
        world
*/

void push_token(t_token *tk, t_token *new_tk)
{
    t_token *tk_next;
    t_token *iter;

    tk_next = (tk)->next;
    (tk)->next = new_tk;
    iter = new_tk;
    while (iter->next)
        iter = iter->next;
    iter->next = tk_next;
}

//  SIGNALS
t_token *heredoc(t_token *tk, int *exit_status)
{
    t_token *iter;
    t_token *res_tk;
    (void)exit_status;

    iter = tk;
    while (iter->next)
    {
        if (iter->type == HEREDOC)
        {
            res_tk = handle_heredoc(iter->next);
            if (!res_tk)
            {
                ft_putstr_fd("Heredoc error\n", 2);
                free_token_list(&tk);
                return NULL;
            }
            // ADD RES_TK TO THE MIDDLE OF << AND NEXT TOKEN AFTER DELIMITER
            push_token(iter->next, res_tk);
        }
        iter = iter->next;
    }
    print_token_list(tk);
    return tk;
}
