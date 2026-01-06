#include "exec.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define FORKED_CHILD 0

typedef struct s_context {
	int fd[2];
	int fd_close;
}t_context;

void exec_node(t_node *node, t_context *ctx);

// Only works if parsed to binary tree?
static void exec_command(t_node *node, t_context *ctx)
{
	// handle error
	if(fork() == FORKED_CHILD) 
	{
		// Evaluate the context
		dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO);
		dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO);
		if(ctx->fd_close >= 0) 
			close(ctx->fd_close);


		char *argv[2] = {"ls", 0};
		execvp(argv[o], argv);
	}
	wait(NULL);
}

void exec_pipe(t_node *node, t_context *ctx)
{
	int p[2];

	pipe(p);
	 
	// additinal context
	t_node *lhs = node->data.pipe.left;
	t_context lhs_ctx = *ctx;
	lhs_ctx.fd[STDOUT_FILENO] = p[STDOUT_FILENO];
	lhs_ctx.fd_close = lhc_ctx.fd[STDIN_FILENO];
	exec_node(lhs, lhs_ctx);

	t_node *rhs = node->data.pipe.right;
	t_context rhs_ctx = *ctx;
	rhs_ctx.fd[STDIN_FILENO] = p[STDIN_FILENO];
	rhs_ctx.fd_close = rhs_ctx.fd[STDOUT_FILENO];
	exec_node(rhs, rhs_ctx);

	// close fd-s in parent
	close(p[STDIN_FILENO]);
	close(p[STDOUT_FILENO]);
}

void exec_node(t_node *node, t_context *ctx)
{
	switch (node->type) {
		case COMMAND_NODE:
			exec_command(node, ctx);
			break;
		case PIPE_NODE:
			exec_pipe(node, ctx);
			break;
		default:
			printf("TODO");
			break;
	}
}

void exec(t_node *node)
{
	t_context ctx = {
		{
			STDIN_FILENO,
			STDOUT_FILENO
		},
		-1
	}
	exec_node(node, &ctx);
}






















