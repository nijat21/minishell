/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 22:34:23 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/16 23:07:27 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_STRUCTS_H
# define PARSE_STRUCTS_H


# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include "libft.h"

# define EXIT_MISUSE 2
# define EXIT_NOT_EXEC 126
# define EXIT_NOT_FOUND 127
# define EXIT_SIGINT 130
# define EXIT_SIGQUIT 131

typedef enum e_sig_src
{
	S_PARENT,
	S_CHILD,
	S_HEREDOC,
	S_NONE,
} t_sig_src;

typedef enum e_tctx
{
	T_NONE,
	T_WORD,
	T_PIPE,
	T_REDIRS,
	T_UNCLOSED_QUOTE,
} t_tctx;

// FROM EXECUTION
typedef enum e_redir_type
{
	REDIR_INPUT = 1,
	REDIR_OUTPUT = 2,
	REDIR_APPEND = 3,
	REDIR_HEREDOC = 4
} t_redir_type;

typedef struct s_redirection
{
	t_redir_type type;
	char *redir_arg;
	bool has_quote;
	int write_fd;
	int read_fd;
	struct s_redirection *next;
} t_redir;

typedef struct s_comand
{
	char *comand;
	char **args;
	t_redir *redir;
	struct s_comand *next;
} t_cmd;


//-LEXER STRUCTURES-------------------------------------------------------------
typedef enum e_quote
{
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE
} t_quote;

typedef enum e_ttype
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	UNCLOSED_QUOTE,
} t_ttype;

typedef struct s_seg
{
	char *val;
	bool expand;
	bool has_quote;
	struct s_seg *next;
} t_seg;

typedef struct s_token
{
	t_ttype type;
	t_seg *seg_list;
	struct s_token *next;
} t_token;

typedef struct s_lex_ctx
{
	t_token *tk;
	t_seg *seg;
	const char *start;
	t_ttype tt;
	t_quote qc;
	size_t len;
	bool has_quote;
} t_lex_ctx;
// -----------------------------------------------------------------------------


#endif