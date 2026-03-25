#include <parser.h>
#include <utils.h>

void	print_unclosed_quote(void)
{
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd("unclosed quote", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_syntax_error(const char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd((char *)str, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

void	print_heredoc_eof_warning(const char *str, int line)
{
	char *line_number;

	line_number = NULL;
	ft_putstr_fd("minishell: warning: here-document", STDERR_FILENO);
	line_number = int_to_ascii(line);
	if (line_number != NULL)
	{
		ft_putstr_fd(" at line ", STDERR_FILENO);
		ft_putstr_fd(line_number, STDERR_FILENO);
		free(line_number);
	}
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd((char *)str, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}