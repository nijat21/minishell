#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#define EXIT_MISUSE 2
#define EXIT_NOT_EXEC 126
#define EXIT_NOT_FOUND 127
#define EXIT_SIGINT 130
#define EXIT_SIGQUIT 131

typedef enum e_parse_stat
{
	PARSE_SUCCESS,
	BAD_INPUT,
	PARSE_FAIL
} t_parse_stat;

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

// ===== parser.c =========================================================
t_parse_stat parse(t_all *all);

// ===== signal.c =========================================================
void set_signal(t_sig_src src);

// ===== suntax_check.c =========================================================
int syntax_check(t_token *tk);
t_tctx ttype_to_tctx(t_ttype t_tt);

// ===== syntax_errors.c =========================================================
void print_syntax_error(const char *str);
void print_unclosed_quote();
void print_heredoc_eof_warning(const char *str);

// ===== print_command.c =========================================================
void print_command(t_cmd *cmd);

// ===== pipeline.c =========================================================
t_cmd **build_pipeline(t_cmd **cmd, t_token *tk, t_all *all);

// ===== pipeline_ops.c =========================================================
void *free_arr_cmdlst(t_cmd **cmd, char **args);
void *free_arg_cmdlst(t_cmd **cmd, char *arg);
char *seg_to_str(t_seg *seg, t_all *all);
char **word_tokens_to_args(t_token **tk, t_all *all);

// ===== pipeline_utils.c =========================================================
t_redir_type ttype_to_redir_type(t_ttype type);
size_t count_word_tokens(t_token *tk);
char **join_args(char **args, char **new_args);
void free_arr(char **arr);

// ===== cmd_list_ops.c =========================================================
t_cmd *command_lstnew(char *comand, char **args);
void command_lstadd_back(t_cmd **lst, t_cmd *new);
void command_lstclear(t_cmd **lst);

// ===== redir_list_ops.c =========================================================
t_redir *redir_lstnew(t_redir_type type, char *arg, bool has_quote);
void redir_lstadd_back(t_redir **lst, t_redir *new);
void redir_lstclear(t_redir **lst);

// ===== expand.c =========================================================
char *expand_var(const char *str, t_all *all);
char *expand_redir_var(t_redir *redir, t_all *all, int fd, char *line);

#endif
