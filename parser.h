#ifndef PARSER_H
#define PARSER_H

#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>

#include "lexer.h"
// #include "data.h" // add after combining with execution

extern int g_signal;

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
	struct s_redirection *next;
} t_redirection;

typedef struct s_comand
{
	char *comand;
	char **args;
	t_redirection *redir;
	struct s_comand *next;
} t_comand;

// Functionalities
int syntax_check(t_token *tk);
t_token *heredoc(t_token *tk, int *exit_status);
void *parse_tokens(t_token *tk, int *exit_status);

// util
t_tctx ttype_to_tctx(t_ttype t_tt);
void print_syntax_error(const char *str);
void print_unclosed_quote();
t_token *heredoc_lexer(t_token **tk, const char *str, bool exp);

// command builder
t_comand *build_pipeline(t_token *tk);
// pipeline utils
t_redir_type ttype_to_redir_type(t_ttype type);
size_t count_word_tokens(t_token *tk);
char **join_args(char **args, char **new_args);
void free_arr(char **arr);
// temporary
void print_comand(t_comand *cmd);

// heredoc utils
t_token *handle_heredoc(t_token *here_tk);
void push_token(t_token *tk, t_token *new_tk);

// command and redir ops
t_comand *command_lstnew(char *comand, char **args);
void command_lstadd_back(t_comand **lst, t_comand *new);
void command_lstclear(t_comand **lst);
t_redirection *redir_lstnew(t_redir_type type, char *arg);
void redir_lstadd_back(t_redirection **lst, t_redirection *new);
void redir_lstclear(t_redirection **lst);

#endif
