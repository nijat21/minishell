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
