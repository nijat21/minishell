#ifndef EXEC_H
#define EXEC_H

#include "Node.h"

typedef struct s_cmd {
	char **argv;
	int fd_in;
	int fd_out;
	s_cmd *next;
}t_cmd;


typedef enum s_node_type {
	COMMAND_NODE,
	PIPE_NODE,
	REDIR_NODE
		// more
} e_node_type;


typedef struct s_pipe {
	t_node *left;
	t_node *right;
} t_pipe;


typedef struct s_cmd {
	char **argv;
} t_cmd;


typedef struct s_redir {
	int fd;
	char *file;
} t_redir;


typedef union  u_data {
	t_pipe pipe;
	t_cmd cmd;
	t_redir redir;
} t_data;


typedef struct s_node {
	t_data data;
	e_node_type type;
} t_node;


void exec(t_node *node);

#endif
