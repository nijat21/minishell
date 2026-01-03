#include "minishell.h"

char *resolve_path(char **env, char *cmd)
{
    int i;
    char **paths;
    char *full_path;

    i = 0;
    while (env[i] && ft_strncmp(env[i], "PATH=", 5))
        i++;
    if (!env[i])
        return NULL;
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return cmd;
        return NULL;
    }
    paths = ft_split(env[i] + 5, ':');
    i = -1;
    while (paths[++i])
    {
        full_path = ft_strjoin3(paths[i], "/", cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(paths);
            return full_path;
        }
    }
    free(paths);
    return NULL;
}

void free_str_arr(char **arr)
{
    int i;

    i = -1;
    while (arr[++i])
        free(arr[i]);
    free(arr);
}

void free_int_arr(int **arr, int len)
{
    int i;

    i = -1;
    while (++i < len)
        free(arr[i]);
    free(arr);
}

