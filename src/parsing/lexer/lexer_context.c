/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_context.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 19:09:11 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/16 22:43:41 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <lexer.h>

void quote_context(const char c, t_quote *qc)
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

void choose_ttype(const char *str, t_ttype *tt)
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

static size_t varname_len(const char *str)
{
	size_t len;
	int i;

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

int add_var(t_lex_ctx *ctx, const char *start, int *i)
{
	size_t var_len;

	var_len = varname_len(start);
	if (!var_len)
		return 0;
	if (!if_len_add_seg(ctx, false))
		return -1;
	if (!add_segment(ctx, start, var_len, true))
		return -1;
	*i += var_len - 1;
	return 1;
}