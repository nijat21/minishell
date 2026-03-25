/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 23:13:23 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 06:34:08 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/select.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define BUFFER_SZ 25
# define CHILD 0
# define ENV_INCREMENT 10
# define FAIL -1
# define STD_TEMP "/tmp/minishell_std_storage"
# define STD_TEMP_LOCATION "/tmp/minishell_std_storage"
# define MAX_LONG_STR "9223372036854775807"
# define MIN_LONG_STR "-9223372036854775808"
#define PATH_BACKUP "/usr/local/sbin:" \
                    "/usr/local/bin:" \
                    "/usr/sbin:" \
                    "/usr/bin:" \
                    "/sbin:" \
                    "/bin"
# define NO_FORK -2

extern int				g_signal;

typedef struct s_gal
{
	int					index1;
	int					index2;
	int					readbytes;
	int					all_read;
	char				*line;
	char				*new_line;
	int					fd;
}						t_gal;

typedef struct s_gnl
{
	int					start;
	int					end;
	int					readbytes;
	char				buffer[BUFFER_SZ + 1];
}						t_gnl;

typedef enum e_redir_type
{
	REDIR_INPUT = 1,
	REDIR_OUTPUT = 2,
	REDIR_APPEND = 3,
	REDIR_HEREDOC = 4
}						t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*redir_arg;
	bool				has_quote;
	struct s_redir		*next;
}						t_redir;

typedef struct s_cmd
{
	char				*comand;
	char				**args;
	t_redir				*redir;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_env
{
	char				**envp;
	int					size;
	int					capacity;
}						t_env;

typedef struct s_fds
{
	int					previous_0;
	int					pipe[2];
	int					io[2];
	int					std_backup[2];
	int					dev_null;
}						t_fds;

typedef struct s_proc
{
	int					signal;
	int					is_heredoc;
	int					exit_status;
}						t_proc;

typedef struct termios	t_term;

typedef struct s_hdoc
{
	char				**temps;
	int					index;
	int					count;
}						t_hdoc;

typedef struct s_all
{
	int					argc;
	char				**argv;
	char				**envp;
	char				*main_line;
	long				main_line_count;
	int					env_status;
	t_env				*my_env;
	t_fds				*fds;
	t_proc				*process_info;
	t_term				saved_termios;
	t_hdoc				heredoc;
	t_cmd				*head;
	int					lst_size;
	int					*children_pids;
	int					node_nbr;
	char				buffer[PATH_MAX + 1];

}						t_all;

typedef					int(func_ptr)(t_all *all, t_cmd *node, t_env *env,
							char *buffer);

typedef struct s_origin
{
	func_ptr			*builtin;
	char				*abs_path;
}						t_origin;

#endif
