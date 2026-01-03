#include "minishell.h"

void exec_cmd(t_pipex *px, char **env, bool last)
{
	char *path;
	int output;

	if (last)
		output = px->fd_out;
	else
		output = px->pipefd[1];
	if (dup2(px->fd_in, STDIN_FILENO) < 0 || dup2(output, STDOUT_FILENO) < 0)
	{
		perror("Dup2");
		exit(EXIT_FAILURE);
	}
	close(px->fd_in);
	close(px->fd_out);
	if (!last)
	{
		close(px->pipefd[0]);
		close(px->pipefd[1]);
	}
	path = resolve_path(env, px->cmd[0]);
	if (!path)
		error_exit(ft_strjoin3("Error: Command not found: ", px->cmd[0], "\n"), true, false);
	execve(path, px->cmd, env);
	shell_exec_error(px->cmd[0]);
}

void setup_child(t_pipex *px, char *av, int i)
{
	bool has_pipe;

	has_pipe = i < px->npipes;
	px->cmd = ft_split(av[i + 2], ' ');
	if (!px->cmd)
	{
		free_str_arr(px->cmd);
		free_px(px);
		error_exit("Cmd split", true, true);
	}
	if (has_pipe)
		if (pipe(px->pipefd) < 0)
		{
			free_str_arr(px->cmd);
			free_px(px);
			error_exit("Pipe", true, true);
		}
	px->pid_ch[i] = fork();
	if (px->pid_ch[i] < 0)
	{
		free_str_arr(px->cmd);
		free_px(px);
		error_exit("Fork", true, true);
	}
}

void clean_parent(t_pipex *px, int i)
{
	free_str_arr(px->cmd);
	close(px->fd_in);
	if (i == px->npipes)
	{
		close(px->fd_out);
		close(px->pipefd[0]);
	}
	else
	{
		close(px->pipefd[1]);
		px->fd_in = px->pipefd[0];
	}
}

void run_processes(int ac, char **av, char **env)
{
	t_pipex *px;
	int status_ch;
	int i;
	bool has_pipe;

	px = parse_input(ac, av);
	i = -1;
	while (++i < px->ncmds)
	{
		setup_child(px, av, i);
		if (px->pid_ch[i] == 0)
			exec_cmd(px, env, i == px->npipes);
		else
			clean_parent(px, i);
	}
	i = -1;
	while (++i < px->ncmds)
		status_ch = safe_waitpid(px->pid_ch[i]);
	free_px(px);
	exit(status_ch);
}

int main(int ac, char **av, char **env)
{
	if (ac < 5)
		return 0;

	run_processes(ac, av, env);

	return 0;
}
