#include <built-ins.h>
#include <parser.h>

// segments of token 1. user, exp=false, 2. $VAR, exp=true -> only expand seg 2
char	*expand_var(const char *str, t_all *all)
{
	char	*var;
	char	*res;

	if (ft_strncmp(str, "$?", 3) == 0)
		return (ft_itoa(all->process_info->exit_status));
	var = ft_strchr(str, '$');
	var++;
	if (!all->my_env || !all->my_env->envp)
	{
		ft_putstr_fd("expand_var: !all->my_env || !all->my_env->envp\n",
			STDERR_FILENO);
		return (NULL);
	}
	res = env_value_dup(var, all->my_env->envp);
	if (!res)
		return (ft_strdup(""));
	return (res);
}

static char	*write_line(char *line, int fd)
{
	ft_putstr_fd(line, fd);
	write(fd, "\n", 1);
	return (line);
}

static char	*next_var(char *line)
{
	char	*n_var;

	if (!line || !line[0])
		return (NULL);
	line++;
	n_var = ft_strchr(line, '$');
	if (n_var)
		return (n_var);
	return (ft_strchr(line, '\0'));
}

static int	expand_each(t_all *all, int fd, char **line)
{
	char	*head;
	size_t	len;
	char	*var;
	char	*temp;

	head = ft_strchr(*line, '$');
	len = varname_len(head);
	if (!head || !len)
	{
		var = next_var(*line);
		write(fd, *line, var - *line);
		*line = var;
		return (0);
	}
	var = safe_malloc(sizeof(char) * (len + 1), "expand_each");
	if (!var)
		return (1);
	write(fd, *line, head - *line);
	ft_strlcpy(var, head, len + 1);
	temp = expand_var(var, all);
	free(var);
	ft_putstr_fd(temp, fd);
	free(temp);
	*line = head + len;
	return (0);
}

char	*expand_redir_var(t_redir *redir, t_all *all, int fd, char *line)
{
	char	*new_line;
	int		res;
	char	*temp;

	if (redir->has_quote || !ft_strchr(line, '$'))
		return (write_line(line, fd));
	new_line = line;
	while (new_line)
	{
		res = expand_each(all, fd, &new_line);
		if (res)
			return (NULL);
		if (!new_line)
			break ;
		temp = ft_strchr(new_line, '$');
		if (!temp)
		{
			ft_putstr_fd(new_line, fd);
			break ;
		}
	}
	ft_putstr_fd("\n", fd);
	return (line);
}
