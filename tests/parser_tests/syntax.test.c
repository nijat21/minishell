#include "tests.h"

typedef struct s_syntax_test
{
    const char *input;
    int expected_exit_code;
    const char *expected_error_msg;
} t_syntax_test;

static char *capture_stderr(int (*fn)(t_token *), t_token *tk, int *exit_code)
{
    int pipefd[2];
    int saved_stderr;
    char buf[4096];
    ssize_t n;
    char *result;
    char *tmp;
    size_t total;

    // Read from STDERR and reset after
    pipe(pipefd);
    saved_stderr = dup(STDERR_FILENO);
    dup2(pipefd[1], STDERR_FILENO);
    close(pipefd[1]);
    *exit_code = fn(tk);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    total = 0;
    result = calloc(1, 1);
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0)
    {
        tmp = realloc(result, total + (size_t)n + 1);
        if (!tmp)
        {
            free(result);
            close(pipefd[0]);
            return (NULL);
        }
        result = tmp;
        memcpy(result + total, buf, (size_t)n);
        total += (size_t)n;
        result[total] = '\0';
    }
    close(pipefd[0]);
    while (total > 0 && result[total - 1] == '\n')
        result[--total] = '\0';
    if (total == 0)
    {
        free(result);
        result = NULL;
    }
    return (result);
}

void run_syntax_tests(t_syntax_test *tests, size_t n)
{
    size_t i;
    t_token *tk;
    char *msg;
    int exit_code;
    bool ok;
    // test stats
    size_t fail = 0;
    size_t pass = 0;

    for (i = 0; i < n; i++)
    {
        tk = lexer(tests[i].input);
        msg = capture_stderr(syntax_check, tk, &exit_code);
        if (exit_code != tests[i].expected_exit_code)
            ok = false;
        else if (tests[i].expected_error_msg == NULL && msg != NULL)
            ok = false;
        else if (tests[i].expected_error_msg != NULL && msg == NULL)
            ok = false;
        else if (msg && strcmp(msg, tests[i].expected_error_msg) != 0)
            ok = false;
        else
            ok = true;
        // print results
        if (ok)
        {
            pass++;
            // printf(GREEN "Test %zu: PASS\n" RESET, i + 1);
            printf(GREEN "#" RESET);
        }
        else
        {
            fail++;
            printf(RED "Test %zu: FAIL\n" RESET, i + 1);
            printf(MAGENTA "  Input:    %s\n", tests[i].input);
            printf(BLUE "  Expected: \n");
            printf("Exit code: %d\n", exit_code);
            printf("Message: %s", tests[i].expected_error_msg);
            printf("\n" RESET);
            printf(YELLOW "  Got:     \n");
            printf("Exit code: %d\n", exit_code);
            printf("Message: %s", msg);
            printf("\n\n" RESET);
        }
    }
    free(msg);
    printf("\n" RESET);
    printf(MAGENTA "Summary: %zu passed, %zu failed\n" RESET, pass, fail);
}

void test_syntax()
{
    t_syntax_test g_tests[] = {
        /* ======================= Faulty syntax tests ======================= */

        /* Pipes */
        {"|", 2, "minishell: syntax error near unexpected token `|'"},
        {"||", 2, "minishell: syntax error near unexpected token `|'"},
        {"|||", 2, "minishell: syntax error near unexpected token `|'"},
        {"ls |", 2, "minishell: syntax error near unexpected token `newline'"},
        {"| ls", 2, "minishell: syntax error near unexpected token `|'"},
        {"ls | | wc", 2, "minishell: syntax error near unexpected token `|'"},
        {"ls | | | wc", 2, "minishell: syntax error near unexpected token `|'"},
        {"| | |", 2, "minishell: syntax error near unexpected token `|'"},

        /* Redirections without filename */
        {"ls >", 2, "minishell: syntax error near unexpected token `newline'"},
        {"ls >>", 2, "minishell: syntax error near unexpected token `newline'"},
        {"ls <", 2, "minishell: syntax error near unexpected token `newline'"},
        {"ls <<", 2, "minishell: syntax error near unexpected token `newline'"},

        /* Redirection followed by another operator */
        {"ls > >", 2, "minishell: syntax error near unexpected token `>'"},
        {"ls < <", 2, "minishell: syntax error near unexpected token `<'"},
        {"ls >> >>", 2, "minishell: syntax error near unexpected token `>>'"},
        {"ls << <<", 2, "minishell: syntax error near unexpected token `<<'"},

        /* Redirection followed by pipe */
        {"ls < |", 2, "minishell: syntax error near unexpected token `|'"},
        {"ls > |", 2, "minishell: syntax error near unexpected token `|'"},
        {"ls >> |", 2, "minishell: syntax error near unexpected token `|'"},
        {"ls << |", 2, "minishell: syntax error near unexpected token `|'"},

        /* Unclosed quotes */
        {"echo \"hello", 2, "minishell: syntax error: unclosed quote"},
        {"echo 'hello", 2, "minishell: syntax error: unclosed quote"},
        {"cat \"file", 2, "minishell: syntax error: unclosed quote"},
        {"cat 'file", 2, "minishell: syntax error: unclosed quote"},
        {"ls \"'mixed", 2, "minishell: syntax error: unclosed quote"},

        /* Pipes + quotes */
        {"ls | echo \"hello", 2, "minishell: syntax error: unclosed quote"},
        {"echo \"hello | wc", 2, "minishell: syntax error: unclosed quote"},

        /* Only redirection without filename and pipe */
        {">", 2, "minishell: syntax error near unexpected token `newline'"},
        {"<", 2, "minishell: syntax error near unexpected token `newline'"},
        {">>", 2, "minishell: syntax error near unexpected token `newline'"},
        {"<<", 2, "minishell: syntax error near unexpected token `newline'"},

        /* Redirection then pipe then nothing */
        {"> |", 2, "minishell: syntax error near unexpected token `|'"},
        {"< |", 2, "minishell: syntax error near unexpected token `|'"},
        {">> |", 2, "minishell: syntax error near unexpected token `|'"},
        {"<< |", 2, "minishell: syntax error near unexpected token `|'"},

        /* Multiple pipes and redirections in sequence */
        {"ls | | > file", 2, "minishell: syntax error near unexpected token `|'"},
        {"ls | > | wc", 2, "minishell: syntax error near unexpected token `|'"},
        {"ls | > >", 2, "minishell: syntax error near unexpected token `>'"},
        {"| > file", 2, "minishell: syntax error near unexpected token `|'"},

        /* Some edge cases */
        {"ls >  ", 2, "minishell: syntax error near unexpected token `newline'"},
        {"ls <   ", 2, "minishell: syntax error near unexpected token `newline'"},
        {"ls >>   ", 2, "minishell: syntax error near unexpected token `newline'"},
        {"ls <<   ", 2, "minishell: syntax error near unexpected token `newline'"},
        {"ls > < file", 2, "minishell: syntax error near unexpected token `<'"},
        {"ls >> << EOF", 2, "minishell: syntax error near unexpected token `<<'"},
        {"ls | >", 2, "minishell: syntax error near unexpected token `newline'"},
        {"ls | >>", 2, "minishell: syntax error near unexpected token `newline'"},
        {"cat 'file < file2", 2, "minishell: syntax error: unclosed quote"},

        /* ======================= Valid syntax tests ======================= */
        {"", 0, NULL},
        {"ls", 0, NULL},
        {"ls | wc", 0, NULL},
        {"ls -l", 0, NULL},
        {"ls -l | wc -l", 0, NULL},
        {"ls > file", 0, NULL},
        {"ls >> file", 0, NULL},
        {"ls < file", 0, NULL},
        {"ls << EOF", 0, NULL},
        {"> file", 0, NULL},
        {">> file", 0, NULL},
        {"< file", 0, NULL},
        {"<< EOF", 0, NULL},
        {"ls | > file", 0, NULL},
        {"> file | ls", 0, NULL},
        {"ls -l | wc > output.txt", 0, NULL},
        {"cat < input.txt | grep 'pattern' > out.txt", 0, NULL},
        {"echo $HOME", 0, NULL},
        {"echo \"hello world\"", 0, NULL},
        {"echo 'hello world'", 0, NULL},
    };
    size_t n = sizeof(g_tests) / sizeof(t_syntax_test);
    run_syntax_tests(g_tests, n);
}