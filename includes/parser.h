#ifndef PARSER_H
# define PARSER_H

# include "parse_structs.h"
// #include <unistd.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <stdbool.h>
// #include <errno.h>
// #include "lexer.h"

// # define EXIT_MISUSE 2
// # define EXIT_NOT_EXEC 126
// # define EXIT_NOT_FOUND 127
// # define EXIT_SIGINT 130
// # define EXIT_SIGQUIT 131

// typedef enum e_sig_src
// {
// 	S_PARENT,
// 	S_CHILD,
// 	S_HEREDOC,
// 	S_NONE,
// } t_sig_src;

// typedef enum e_tctx
// {
// 	T_NONE,
// 	T_WORD,
// 	T_PIPE,
// 	T_REDIRS,
// 	T_UNCLOSED_QUOTE,
// } t_tctx;

// // FROM EXECUTION
// typedef enum e_redir_type
// {
// 	REDIR_INPUT = 1,
// 	REDIR_OUTPUT = 2,
// 	REDIR_APPEND = 3,
// 	REDIR_HEREDOC = 4
// } t_redir_type;

// typedef struct s_redirection
// {
// 	t_redir_type type;
// 	char *redir_arg;
// 	bool has_quote;
// 	int write_fd;
// 	int read_fd;
// 	struct s_redirection *next;
// } t_redir;

// typedef struct s_comand
// {
// 	char *comand;
// 	char **args;
// 	t_redir *redir;
// 	struct s_comand *next;
// } t_cmd;


// signal
void set_signal(t_sig_src src);

// Functionalities
t_token *lexer(const char *prompt);
int syntax_check(t_token *tk);
t_cmd *parse_tokens(char *line, int *exit_status);
void run_heredocs(t_cmd *cmd);
int heredoc(t_cmd *cmd);

// util
t_tctx ttype_to_tctx(t_ttype t_tt);
void print_syntax_error(const char *str);
void print_unclosed_quote();
void print_heredoc_eof_warning(const char *str);
int heredoc_status(bool debug, char *str, int exit_status);

// command builder
t_cmd **build_pipeline(t_cmd **cmd, t_token *tk);

// expand
char *expand_var(char *str);
char *expand_redir_arg(const char *str);

// pipeline operations
void *free_arr_cmdlst(t_cmd **cmd, char **args);
void *free_arg_cmdlst(t_cmd **cmd, char *arg);
char *seg_to_str(t_seg *seg);
char **word_tokens_to_args(t_token **tk);

// pipeline utils
t_redir_type ttype_to_redir_type(t_ttype type);
size_t count_word_tokens(t_token *tk);
char **join_args(char **args, char **new_args);
void free_arr(char **arr);
// temporary
void print_comand(t_cmd *cmd);

// command and redir ops
t_cmd *command_lstnew(char *comand, char **args);
void command_lstadd_back(t_cmd **lst, t_cmd *new);
void command_lstclear(t_cmd **lst);
t_redir *redir_lstnew(t_redir_type type, char *arg, bool has_quote);
void redir_lstadd_back(t_redir **lst, t_redir *new);
void redir_lstclear(t_redir **lst);

// clean
void close_write_fds(t_cmd *cmd);

#endif
