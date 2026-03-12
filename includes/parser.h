#ifndef PARSER_H
#define PARSER_H

#include <signal.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

#include "lexer.h"

#define EXIT_MISUSE 2
#define EXIT_NOT_EXEC 126
#define EXIT_NOT_FOUND 127
#define EXIT_SIGINT 130
#define EXIT_SIGQUIT 131

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
	bool has_quote;
	int write_fd;
	int read_fd;
	struct s_redirection *next;
} t_redirection;

typedef struct s_comand
{
	char *comand;
	char **args;
	t_redirection *redir;
	struct s_comand *next;
} t_comand;

// signal
void sigint_handler(int sig);
void setup_signals(void);

// Functionalities
int syntax_check(t_token *tk);
t_comand *parse_tokens(t_token *tk, int *exit_status);
void run_heredocs(t_comand *cmd);
int heredoc(t_comand *cmd);

// util
t_tctx ttype_to_tctx(t_ttype t_tt);
void print_syntax_error(const char *str);
void print_unclosed_quote();
void print_heredoc_eof_warning(const char *str);

// command builder
t_comand **build_pipeline(t_comand **cmd, t_token *tk);

// expand
char *expand_var(char *str);
char *expand_redir_arg(const char *str);

// pipeline operations
void *free_arr_cmdlst(t_comand **cmd, char **args);
void *free_arg_cmdlst(t_comand **cmd, char *arg);
char *seg_to_str(t_seg *seg);
char **word_tokens_to_args(t_token **tk);

// pipeline utils
t_redir_type ttype_to_redir_type(t_ttype type);
size_t count_word_tokens(t_token *tk);
char **join_args(char **args, char **new_args);
void free_arr(char **arr);
// temporary
void print_comand(t_comand *cmd);

// command and redir ops
t_comand *command_lstnew(char *comand, char **args);
void command_lstadd_back(t_comand **lst, t_comand *new);
void command_lstclear(t_comand **lst);
t_redirection *redir_lstnew(t_redir_type type, char *arg, bool has_quote);
void redir_lstadd_back(t_redirection **lst, t_redirection *new);
void redir_lstclear(t_redirection **lst);

#endif
