#include "tests.h"

/* ─── Expected-value structs ─────────────────────────────────────────────── */

typedef struct s_redir_test
{
	t_redir_type	type;
	const char		*redir_arg;
	bool			has_quote;
}					t_redir_test;

typedef struct s_cmd_test
{
	const char		*comand;
	const char **args; /* NULL-terminated */
	t_redir_test	*redirs;
	int				redir_count;
}					t_cmd_test;

typedef struct s_pipeline_test
{
	const char		*input;
	int exit_status; /* loaded into all->process_info->exit_status */
	bool			expect_syntax_fail;
	t_cmd_test		*expected_cmds;
	int				expected_cmd_count;
}					t_pipeline_test;

/* ─── make_all ───────────────────────────────────────────────────────────── */

static t_all	*make_all(int exit_status)
{
	t_all	*all;
	t_proc	*proc;

	all = calloc(1, sizeof(t_all));
	proc = calloc(1, sizeof(t_proc));
	proc->exit_status = exit_status;
	all->process_info = proc;
	return (all);
}

/* ─── run ────────────────────────────────────────────────────────────────── */

static t_cmd	*run(const char *input, t_all *all, int *syntax_ret)
{
	t_token	*tk;
	t_cmd	*cmd;

	tk = lexer(input);
	*syntax_ret = syntax_check(tk);
	if (*syntax_ret != 0)
		return (NULL);
	cmd = NULL;
	build_pipeline(&cmd, tk, all);
	return (cmd);
}

/* ─── compare ────────────────────────────────────────────────────────────── */

static bool	compare_pipeline(t_cmd *actual, t_cmd_test *expected,
		int expected_len)
{
	int		i;
	int		j;
	t_redir	*r;

	i = 0;
	while (actual && i < expected_len)
	{
		/* comand */
		if (!actual->comand || strcmp(actual->comand, expected[i].comand) != 0)
			return (false);
		/* args */
		j = 0;
		while (expected[i].args[j])
		{
			if (!actual->args || !actual->args[j])
				return (false);
			if (strcmp(actual->args[j], expected[i].args[j]) != 0)
				return (false);
			j++;
		}
		if (actual->args && actual->args[j])
			return (false); /* extra args */
		/* redirs */
		r = actual->redir;
		for (j = 0; j < expected[i].redir_count; j++)
		{
			if (!r)
				return (false);
			if (r->type != expected[i].redirs[j].type)
				return (false);
			if (!r->redir_arg || strcmp(r->redir_arg,
					expected[i].redirs[j].redir_arg) != 0)
				return (false);
			if (r->has_quote != expected[i].redirs[j].has_quote)
				return (false);
			r = r->next;
		}
		if (r)
			return (false); /* extra redirs */
		actual = actual->next;
		i++;
	}
	if (actual || i != expected_len)
		return (false);
	return (true);
}

/* ─── print helpers ──────────────────────────────────────────────────────── */

static const char	*rtype(t_redir_type t)
{
	if (t == REDIR_INPUT)
		return ("REDIR_INPUT");
	if (t == REDIR_OUTPUT)
		return ("REDIR_OUTPUT");
	if (t == REDIR_APPEND)
		return ("REDIR_APPEND");
	if (t == REDIR_HEREDOC)
		return ("REDIR_HEREDOC");
	return ("UNKNOWN");
}

static void	print_expected(t_cmd_test *cmds, int n)
{
	int	i;
	int	j;

	for (i = 0; i < n; i++)
	{
		printf("    cmd[%d]: comand=\"%s\" args=[", i, cmds[i].comand);
		for (j = 0; cmds[i].args[j]; j++)
			printf(j ? ", %s" : "%s", cmds[i].args[j]);
		printf(", NULL]");
		if (cmds[i].redir_count)
		{
			printf(" redirs=[");
			for (j = 0; j < cmds[i].redir_count; j++)
				printf(j ? ", {%s \"%s\" q:%s}" : "{%s \"%s\" q:%s}",
					rtype(cmds[i].redirs[j].type), cmds[i].redirs[j].redir_arg,
					cmds[i].redirs[j].has_quote ? "yes" : "no");
			printf("]");
		}
		printf("\n");
	}
}

static void	print_actual(t_cmd *cmd)
{
	int i;
	int j;
	t_redir *r;

	i = 0;
	while (cmd)
	{
		printf("    cmd[%d]: comand=\"%s\" args=[", i++,
			cmd->comand ? cmd->comand : "(null)");
		if (cmd->args)
		{
			for (j = 0; cmd->args[j]; j++)
				printf(j ? ", %s" : "%s", cmd->args[j]);
		}
		printf(", NULL]");
		r = cmd->redir;
		if (r)
		{
			printf(" redirs=[");
			while (r)
			{
				printf(r != cmd->redir ? ",
					{%s \"%s\" q:%s}" : "{%s \"%s\" q:%s}", rtype(r->type),
					r->redir_arg ? r->redir_arg : "(null)",
					r->has_quote ? "yes" : "no");
					r = r->next;
			}
			printf("]");
			}
			printf("\n");
			cmd = cmd->next;
		}
	}

	/* ─── test runner ────────────────────────────────────────────────────────── */

	/* static void test_pipeline_cases(t_pipeline_test *tests, size_t n)
	{
		size_t	i;
		size_t	pass;
		size_t	fail;
		int		sr;
		t_cmd	*result;
		bool	ok;
		t_all	*all;
		int		saved_stderr;
		size_t	i;
		size_t	pass;
		size_t	fail;
		int		sr;
		t_cmd	*result;
		bool	ok;
		t_all	*all;
			char errbuf[4096];
			int errlen;
			int pipefd[2];
		int		saved_stderr;

		pass = 0;
		fail = 0;
		for (i = 0; i < n; i++)
		{
			all = make_all(tests[i].exit_status);
			fflush(stdout);
			saved_stderr = dup(STDERR_FILENO);
			dup2(STDOUT_FILENO, STDERR_FILENO);
			result = run(tests[i].input, all, &sr);
			fflush(stdout);
			dup2(saved_stderr, STDERR_FILENO);
			close(saved_stderr);
			if (tests[i].expect_syntax_fail)
				ok = (sr != 0 && result == NULL);
			else
				ok = (sr == 0 && compare_pipeline(result,
													tests[i].expected_cmds,
													tests[i].expected_cmd_count));
			if (ok)
			{
				// printf(GREEN "#" RESET);
				printf(GREEN "\nTest %zu: PASS\n" RESET, i + 1);
				pass++;
			}
			else
			{
				fail++;
				printf(RED "\nTest %zu: FAIL\n" RESET, i + 1);
			}
			printf(MAGENTA "  Input:    %s\n" RESET, tests[i].input);
			if (tests[i].expect_syntax_fail)
			{
				printf(BLUE "  Expected: syntax_ret=2, cmd=NULL\n" RESET);
				printf(YELLOW "  Got:      syntax_ret=%d, cmd=%s\n",
						sr, result ? "(non-null)" : "NULL" RESET);
			}
			else
			{
				printf(BLUE "  Expected:\n");
				print_expected(tests[i].expected_cmds,
					tests[i].expected_cmd_count);
				printf(YELLOW "  Got:\n");
				if (result)
					print_actual(result);
				else
					printf("    (null) — syntax_ret=%d\n", sr);
				printf(RESET);
			}
			printf(RESET);
		}
		printf("\n" RESET);
		printf(MAGENTA "Summary: %zu passed, %zu failed\n" RESET, pass, fail);
	} */
	static void test_pipeline_cases(t_pipeline_test * tests, size_t n)
	{
		pass = 0;
		fail = 0;
		for (i = 0; i < n; i++)
		{
			all = make_all(tests[i].exit_status);
			/* capture stderr so we can print it after the test result */
			pipe(pipefd);
			saved_stderr = dup(STDERR_FILENO);
			dup2(pipefd[1], STDERR_FILENO);
			close(pipefd[1]);
			result = run(tests[i].input, all, &sr);
			fflush(stderr);
			dup2(saved_stderr, STDERR_FILENO);
			close(saved_stderr);
			errlen = read(pipefd[0], errbuf, sizeof(errbuf) - 1);
			close(pipefd[0]);
			if (errlen > 0)
				errbuf[errlen] = '\0';
			else
				errbuf[0] = '\0';
			if (tests[i].expect_syntax_fail)
				ok = (sr != 0 && result == NULL);
			else
				ok = (sr == 0 && compare_pipeline(result,
							tests[i].expected_cmds,
							tests[i].expected_cmd_count));
			if (ok)
			{
				printf(GREEN "#" RESET);
				// printf(GREEN "\nTest %zu: PASS\n" RESET, i + 1);
				pass++;
			}
			else
			{
				fail++;
				printf(RED "\nTest %zu: FAIL\n" RESET, i + 1);
				printf(MAGENTA "  Input:    %s\n" RESET, tests[i].input);
				if (tests[i].expect_syntax_fail)
				{
				printf(BLUE "  Expected: syntax_ret=<non-zero>,
					cmd=NULL\n" RESET);
				printf(YELLOW "  Got:      syntax_ret=%d, cmd=%s\n", sr,
					result ? "(non-null)" : "NULL" RESET);
				}
				else
				{
					printf(BLUE "  Expected:\n");
					print_expected(tests[i].expected_cmds,
						tests[i].expected_cmd_count);
					printf(YELLOW "  Got:\n");
					if (result)
						print_actual(result);
					else
						printf("    (null) — syntax_ret=%d\n", sr);
					printf(RESET);
				}
			}
			if (errbuf[0])
				fprintf(stderr, "%s", errbuf);
		}
		printf("\n" RESET);
		printf(MAGENTA "Summary: %zu passed, %zu failed\n" RESET, pass, fail);
	}

	/* ═══════════════════════════════════════════════════════════════════════════ */
	/*  TEST CASES                                                                 */
	/* ═══════════════════════════════════════════════════════════════════════════ */

	void test_pipeline(void)
	{
		t_pipeline_test tests[] = {
			/* ── correct inputs ─────────────────────────────────────────────── */
			{
				.input = "echo hello",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "hello", NULL}, NULL,
							0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "cat < file",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", NULL},
							(t_redir_test[]){{REDIR_INPUT, "file", false}}, 1},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo hi > out",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "hi", NULL},
							(t_redir_test[]){{REDIR_OUTPUT, "out", false}}, 1},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo hi >> out",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "hi", NULL},
							(t_redir_test[]){{REDIR_APPEND, "out", false}}, 1},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "cat << del",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", NULL},
							(t_redir_test[]){{REDIR_HEREDOC, "del", false}}, 1},
					},
				.expected_cmd_count = 1,
			},
			{
				/* the example from your print_comand output */
				.input = "cat << del in > out",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat",
							(const char *[]){"cat", "in", NULL},
							(t_redir_test[]){
								{REDIR_HEREDOC, "del", false},
								{REDIR_OUTPUT, "out", false},
							},
							2},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "ls | grep foo",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"ls", (const char *[]){"ls", NULL}, NULL, 0},
						{"grep", (const char *[]){"grep", "foo", NULL}, NULL,
							0},
					},
				.expected_cmd_count = 2,
			},
			{
				.input = "cat file | sort | uniq",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", "file", NULL}, NULL, 0},
						{"sort", (const char *[]){"sort", NULL}, NULL, 0},
						{"uniq", (const char *[]){"uniq", NULL}, NULL, 0},
					},
				.expected_cmd_count = 3,
			},
			{
				.input = "echo \"hello world\"",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "hello world", NULL},
							NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo > \"my file\"",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", NULL},
							(t_redir_test[]){{REDIR_OUTPUT, "my file", true}},
							1},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "grep foo < in bar",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"grep", (const char *[]){"grep", "foo", "bar", NULL},
							(t_redir_test[]){{REDIR_INPUT, "in", false}}, 1},
					},
				.expected_cmd_count = 1,
			},
			/* ── syntax errors ───────────────────────────────────────────────── */
			{
				.input = "| ls",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				.input = "ls |",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				.input = "ls || grep",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				.input = "echo \"hello",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				.input = "echo 'hello",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				.input = "echo >",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				.input = "echo > | cat",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			/* ── expansions ──────────────────────────────────────────────────── */
			{
				/* unknown var → "" (adjust to NULL-args if your impl drops it) */
				.input = "echo $HOME",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo $?",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "0", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo $?",
				.exit_status = 2,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "2", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				/* $USER → "", $? → "0" */
				.input = "echo $USER $?",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "", "0", NULL}, NULL,
							0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "cat < $FILE",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", NULL},
							(t_redir_test[]){{REDIR_INPUT, "", false}}, 1},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "cat < $?",
				.exit_status = 42,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", NULL},
							(t_redir_test[]){{REDIR_INPUT, "42", false}}, 1},
					},
				.expected_cmd_count = 1,
			},
			/* ── multiple pipes ──────────────────────────────────────────────── */
			{
				.input = "cat file | sort -r | uniq -c | head -5",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", "file", NULL}, NULL, 0},
						{"sort", (const char *[]){"sort", "-r", NULL}, NULL, 0},
						{"uniq", (const char *[]){"uniq", "-c", NULL}, NULL, 0},
						{"head", (const char *[]){"head", "-5", NULL}, NULL, 0},
					},
				.expected_cmd_count = 4,
			},
			{
				.input = "a | b | c | d | e",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"a", (const char *[]){"a", NULL}, NULL, 0},
						{"b", (const char *[]){"b", NULL}, NULL, 0},
						{"c", (const char *[]){"c", NULL}, NULL, 0},
						{"d", (const char *[]){"d", NULL}, NULL, 0},
						{"e", (const char *[]){"e", NULL}, NULL, 0},
					},
				.expected_cmd_count = 5,
			},
			{
				/* redirs on both sides of a pipe */
				.input = "cat < in | grep foo > out",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", NULL},
							(t_redir_test[]){{REDIR_INPUT, "in", false}}, 1},
						{"grep", (const char *[]){"grep", "foo", NULL},
							(t_redir_test[]){{REDIR_OUTPUT, "out", false}}, 1},
					},
				.expected_cmd_count = 2,
			},
			{
				/* heredoc on first, append on last */
				.input = "cat << EOF | sort >> result",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", NULL},
							(t_redir_test[]){{REDIR_HEREDOC, "EOF", false}}, 1},
						{"sort", (const char *[]){"sort", NULL},
							(t_redir_test[]){{REDIR_APPEND, "result", false}},
							1},
					},
				.expected_cmd_count = 2,
			},
			/* ── multiple redirections on one command ────────────────────────── */
			{
				.input = "cat < in > out",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat",
							(const char *[]){"cat", NULL},
							(t_redir_test[]){
								{REDIR_INPUT, "in", false},
								{REDIR_OUTPUT, "out", false},
							},
							2},
					},
				.expected_cmd_count = 1,
			},
			{
				/* two output redirs — last one wins at runtime, both stored */
				.input = "echo hi > a > b",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo",
							(const char *[]){"echo", "hi", NULL},
							(t_redir_test[]){
								{REDIR_OUTPUT, "a", false},
								{REDIR_OUTPUT, "b", false},
							},
							2},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "cat < in >> out",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat",
							(const char *[]){"cat", NULL},
							(t_redir_test[]){
								{REDIR_INPUT, "in", false},
								{REDIR_APPEND, "out", false},
							},
							2},
					},
				.expected_cmd_count = 1,
			},
			/* ── single quoted redir arg ─────────────────────────────────────── */
			{
				.input = "echo > 'my file'",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", NULL},
							(t_redir_test[]){{REDIR_OUTPUT, "my file", true}},
							1},
					},
				.expected_cmd_count = 1,
			},
			/* ── many args ───────────────────────────────────────────────────── */
			{
				.input = "echo a b c d e",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "a", "b", "c", "d",
							"e", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── no-arg commands ─────────────────────────────────────────────── */
			{
				.input = "pwd",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"pwd", (const char *[]){"pwd", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── single-quoted arg preserves spaces ──────────────────────────── */
			{
				.input = "echo 'hello world'",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "hello world", NULL},
							NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── single-quoted arg suppresses expansion ───────────────────────── */
			{
				.input = "echo '$?'",
				.exit_status = 5,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "$?", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── $? expansion with two digits ────────────────────────────────── */
			{
				.input = "echo $?",
				.exit_status = 127,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "127", NULL}, NULL,
							0},
					},
				.expected_cmd_count = 1,
			},
			/* ── $? embedded inside a word ───────────────────────────────────── */
			{
				.input = "echo exit$?code",
				.exit_status = 1,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "exit1code", NULL},
							NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── more syntax errors ──────────────────────────────────────────── */
			{
				/* empty input between two pipes */
				.input = "ls | | grep",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				/* redir with no file, followed by another redir */
				.input = "echo >> >> file",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				/* just a pipe, nothing else */
				.input = "|",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				/* heredoc with no delimiter before pipe */
				.input = "<< | cat",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			{
				/* three pipes in a row */
				.input = "ls ||| grep",
				.exit_status = 0,
				.expect_syntax_fail = true,
			},
			/* ── quoting: pipe and redirs inside quotes become plain args ─────── */
			{
				.input = "echo \"|\"",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "|", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo '|'",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "|", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo \">\"",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", ">", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo '>'",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", ">", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo '<'",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "<", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── quoting: empty strings ──────────────────────────────────────── */
			{
				.input = "echo \"\"",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo ''",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── quoting: adjacent quoted+unquoted segments merge into one arg ── */
			{
				.input = "echo \"hello\"world",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "helloworld", NULL},
							NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo 'hello'world",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "helloworld", NULL},
							NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				/* adjacent single + double quoted segments */
				.input = "echo 'hello'\"world\"",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "helloworld", NULL},
							NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── quoting: command name itself quoted ─────────────────────────── */
			{
				.input = "\"echo\" hello",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "hello", NULL}, NULL,
							0},
					},
				.expected_cmd_count = 1,
			},
			/* ── quoting: single quotes suppress all expansion ───────────────── */
			{
				.input = "echo '$HOME'",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "$HOME", NULL}, NULL,
							0},
					},
				.expected_cmd_count = 1,
			},
			{
				/* single quotes suppress $? too */
				.input = "echo '$?'",
				.exit_status = 99,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "$?", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── quoting: double quotes expand $? but not other vars ─────────── */
			{
				.input = "echo \"$?\"",
				.exit_status = 3,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "3", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "echo \"$HOME\"",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"echo", (const char *[]){"echo", "", NULL}, NULL, 0},
					},
				.expected_cmd_count = 1,
			},
			/* ── quoting: quoted heredoc delimiter suppresses expansion ──────── */
			{
				.input = "cat << 'EOF'",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", NULL},
							(t_redir_test[]){{REDIR_HEREDOC, "EOF", true}}, 1},
					},
				.expected_cmd_count = 1,
			},
			{
				.input = "cat << \"EOF\"",
				.exit_status = 0,
				.expect_syntax_fail = false,
				.expected_cmds =
					(t_cmd_test[]){
						{"cat", (const char *[]){"cat", NULL},
							(t_redir_test[]){{REDIR_HEREDOC, "EOF", true}}, 1},
					},
				.expected_cmd_count = 1,
			},
		};

		test_pipeline_cases(tests, sizeof(tests) / sizeof(t_pipeline_test));
	}