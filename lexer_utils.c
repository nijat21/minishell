#include "minishell.h"

void free_seg_list(t_seg *seg)
{
	t_seg *temp;

	while(seg)
	{
		temp = seg->next;
		free(seg->val);
		free(seg);
		seg = temp;
	}
}

void free_token_list(t_token *tk)
{
	t_token *temp;

	while(tk)
	{
		temp = tk->next;
		free_seg_list(tk->seg_list);
		free(tk);
		tk = temp;
	}
}

void *safe_malloc(size_t bytes)
{
	void *mem;

	mem = malloc(bytes);
	if (!mem)
	{
		perror("Malloc");
		return (NULL);
	}
	return (mem);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v'
		|| c == '\f')
		return (1);
	else
		return (0);
}


void print_token_list(t_token *tk)
{
	t_token *temp;
	t_seg *temp_seg;
	const char *arr[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND", "UNCLOSED_QUOTE"};

	temp = tk;
	while(temp)
	{
		printf("%s", arr[temp->type]);
		temp_seg = temp->seg_list;
		while(temp_seg)
		{
			printf("(%s) exp(%s)\n", temp_seg->val, temp_seg->expand ? "yes" : "no");
			temp_seg = temp_seg->next;
		}
		temp = temp->next;
	}
}

