#include "minishell.h"

void *safe_malloc(size_t bytes)
{
    void *mem;

    mem = malloc(bytes);
    if (!mem)
    {
        error_exit("Malloc", false, true);
        return (NULL);
    }
    return (mem);
}

void safe_dup2(int fd, int fd2)
{
    int res;

    res = dup2(fd, fd2);
    if (res < 0)
        error_exit(ft_strjoin("Dup2: filedes: ", ft_itoa(fd)), true, true);
}

int safe_waitpid(pid_t pid)
{
    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("Waitpid");
        return -1;
    }
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return -1;
}
