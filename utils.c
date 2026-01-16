#include "minishell.h"

void free_list(t_token *tk)
{
	t_token *temp;

	while(tk)
	{
		temp = tk->next;
		free(tk->val);
		free(tk);
		tk = temp;
	}
}
