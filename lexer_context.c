#include "lexer.h"

void	quote_context(const char c, t_quote *qc)
{
	if (*qc == Q_NONE)
	{
		if (c == '\'')
			*qc = Q_SINGLE;
		else if (c == '"')
			*qc = Q_DOUBLE;
	}
	else if (*qc == Q_SINGLE)
	{
		if (c == '\'')
			*qc = Q_NONE;
	}
	else if (*qc == Q_DOUBLE)
	{
		if (c == '"')
			*qc = Q_NONE;
	}
}

void	choose_ttype(const char *str, t_ttype *tt)
{
	if (*str == '|')
		*tt = PIPE;
	else if (*str == '<')
	{
		str++;
		if (*str && *str == '<')
			*tt = HEREDOC;
		else
			*tt = REDIR_IN;
	}
	else if (*str == '>')
	{
		str++;
		if (*str && *str == '>')
			*tt = APPEND;
		else
			*tt = REDIR_OUT;
	}
	else
		*tt = WORD;
}

size_t	varname_len(const char *str)
{
	size_t	len;
	int		i;

	len = 1;
	i = 1;
	if (str[i] && str[i] == '?')
		len++;
	else if (str[i] && (ft_isalpha(str[i]) || str[i] == '_'))
	{
		len++;
		while (str[++i])
		{
			if (!ft_isalnum(str[i]) && str[i] != '_')
				return (len);
			len++;
		}
	}
	else
		return (0);
	return (len);
}
