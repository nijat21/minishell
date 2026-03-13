#include "includes/parser.h"

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

static t_redirection *get_heredoc_fds(t_redirection *redir, int n)
{
    char *num;
    char *path;

    num = ft_itoa(n);
    path = ft_strjoin("/tmp/minishell_heredoc_", num);
    if (!path)
        return NULL;
    free(num);
    redir->write_fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    redir->read_fd = open(path, O_RDONLY);
    unlink(path);
    free(path);
    if (redir->write_fd < 0 || redir->read_fd < 0)
        return NULL;
    return redir;
}

static t_comand *assign_heredoc_fds(t_comand *cmd)
{
    t_comand *cmd_iter;
    t_redirection *redir_iter;
    int n;

    n = 0;
    cmd_iter = cmd;
    while (cmd_iter)
    {
        redir_iter = cmd_iter->redir;
        while (redir_iter)
        {
            if (redir_iter->type == REDIR_HEREDOC)
            {
                if (!get_heredoc_fds(redir_iter, n))
                    return NULL;
                n++;
            }
            redir_iter = redir_iter->next;
        }
        cmd_iter = cmd_iter->next;
    }
    return cmd;
}

static t_redirection *read_from_heredoc_file(t_redirection *redir)
{
    char *new_str;

    if (!redir->redir_arg)
        return NULL;
    free(redir->redir_arg);
    redir->redir_arg = NULL;
    new_str = get_next_line(redir->read_fd);
    while (new_str)
    {
        redir->redir_arg = ft_strjoin_free(redir->redir_arg, new_str);
        new_str = get_next_line(redir->read_fd);
        if (!redir->redir_arg)
            return NULL;
    }
    return redir;
}

static t_comand *update_heredocs_args(t_comand *cmd)
{
    t_comand *cmd_iter;
    t_redirection *redir_iter;

    cmd_iter = cmd;
    while (cmd_iter)
    {
        redir_iter = cmd_iter->redir;
        while (redir_iter)
        {
            if (redir_iter->type == REDIR_HEREDOC)
            {
                if (!read_from_heredoc_file(redir_iter))
                {
                    close(redir_iter->read_fd);
                    return NULL;
                }
                close(redir_iter->read_fd);
            }
            redir_iter = redir_iter->next;
        }
        cmd_iter = cmd_iter->next;
    }
    return cmd;
}

// close pipefds
int heredoc(t_comand *cmd)
{
    pid_t pid;
    int status;

    if (!assign_heredoc_fds(cmd))
        return heredoc_status(true, "assign_heredoc_fds", EXIT_FAILURE);

    pid = fork();
    if (pid < 0)
        return heredoc_status(true, "fork", EXIT_FAILURE);
    if (pid == 0)
    {
        run_heredocs(cmd);
        exit(EXIT_SUCCESS);
    }
    waitpid(pid, &status, 0);
    close_write_fds(cmd);
    // signals(false);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        printf("caught\n");
        // g_signal = 0;
        command_lstclear(&cmd);
        return EXIT_SIGINT;
    }
    else if (WIFEXITED(status) && (WEXITSTATUS(status) != EXIT_SUCCESS))
    {
        command_lstclear(&cmd);
        if (WEXITSTATUS(status) == EXIT_SIGINT)
        {
            g_signal = 0;
            return heredoc_status(true, "sigint", WEXITSTATUS(status));
        }
        return heredoc_status(true, "other", WEXITSTATUS(status));
    }
    if (!update_heredocs_args(cmd))
        return heredoc_status(true, "update_heredocs_args", EXIT_FAILURE);
    return EXIT_SUCCESS;
}
