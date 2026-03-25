#include <parser.h>

t_redir_type	ttype_to_redir_type(t_ttype type)
{
	t_redir_type	res;

	if (type == REDIR_OUT)
		res = REDIR_OUTPUT;
	else if (type == REDIR_IN)
		res = REDIR_INPUT;
	else if (type == APPEND)
		res = REDIR_APPEND;
	else if (type == HEREDOC)
		res = REDIR_HEREDOC;
	else
	{
		ft_putstr_fd("Parser: ttype_to_redir_type", STDERR_FILENO);
		return (-1);
	}
	return (res);
}

size_t	count_word_tokens(t_token *tk)
{
	size_t	count;
	t_token	*temp;

	temp = tk;
	count = 0;
	while (temp && temp->type == WORD)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

void	*free_arr_cmdlst(t_cmd **cmd, char **args)
{
	ft_free_arr(args);
	command_lstclear(cmd);
	return (NULL);
}

void	*free_arg_cmdlst(t_cmd **cmd, char *arg)
{
	free(arg);
	command_lstclear(cmd);
	return (NULL);
}
