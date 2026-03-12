#include "includes/parser.h"

void sigint_handler(int sig)
{
    g_signal = sig;
    ioctl(STDIN_FILENO, TIOCSTI, "\n"); // kernel 6.2+ dropped TIOCSTI
                                        // ft_putchar_fd('\n', STDOUT_FILENO);
                                        // rl_on_new_line();
                                        // rl_replace_line("", 0);
                                        // rl_redisplay();
                                        // rl_done = 1;
}

void setup_signals(void)
{
    struct sigaction sa;

    rl_catch_signals = 0;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}