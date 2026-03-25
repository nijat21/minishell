/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_structures.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:59:44 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/25 02:44:23 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_STRUCTURES_H
# define PARSE_STRUCTURES_H

# define EXIT_MISUSE 2
# define EXIT_NOT_EXEC 126
# define EXIT_NOT_FOUND 127
# define EXIT_SIGINT 130
# define EXIT_SIGQUIT 131

# include <errno.h>
# include <libft.h>
# include <minishell.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum e_parse_stat
{
	PARSE_SUCCESS,
	BAD_INPUT,
	PARSE_FAIL
}					t_parse_stat;

typedef enum e_sig_src
{
	S_PARENT,
	S_CHILD,
	S_HEREDOC,
	S_NONE,
}					t_sig_src;

typedef enum e_tctx
{
	T_NONE,
	T_WORD,
	T_PIPE,
	T_REDIRS,
	T_UNCLOSED_QUOTE,
}					t_tctx;

typedef enum e_quote
{
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE
}					t_quote;

typedef enum e_ttype
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	UNCLOSED_QUOTE,
}					t_ttype;

typedef struct s_seg
{
	char			*val;
	bool			expand;
	bool			has_quote;
	struct s_seg	*next;
}					t_seg;

typedef struct s_token
{
	t_ttype			type;
	t_seg			*seg_list;
	struct s_token	*next;
}					t_token;

typedef struct s_lex_ctx
{
	t_token			*tk;
	t_seg			*seg;
	const char		*start;
	t_ttype			tt;
	t_quote			qc;
	size_t			len;
	bool			has_quote;
}					t_lex_ctx;

#endif
