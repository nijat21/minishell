#include "includes/parser.h"

void main_handler(int sig)
{
    g_signal = sig;
    // ioctl(STDIN_FILENO, TIOCSTI, "\n");
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void heredoc_handler(int sig)
{
    g_signal = sig;
    write(STDOUT_FILENO, "\n", 1);
    // ioctl(STDIN_FILENO, TIOCSTI, "\r");
}

void setup_handler(void (*handler)(int), int sig)
{
    struct sigaction sa;

    rl_catch_signals = 0;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(sig, &sa, NULL);
}

void set_signal(t_sig_src src)
{
    if (src == S_PARENT)
    {
        setup_handler(main_handler, SIGINT);
        setup_handler(SIG_IGN, SIGQUIT);
    }
    else if (src == S_HEREDOC)
    {
        setup_handler(heredoc_handler, SIGINT);
        setup_handler(SIG_IGN, SIGQUIT);
    }
    else if (src == S_CHILD)
    {
        setup_handler(SIG_DFL, SIGINT);
        setup_handler(SIG_IGN, SIGQUIT);
    }
    else
    {
        setup_handler(SIG_DFL, SIGINT);
        setup_handler(SIG_IGN, SIGQUIT);
    }
}