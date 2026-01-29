#include "lexer.h"

static t_seg* seg_init(t_seg **seg, size_t len)
{
	t_seg *new;

	new = safe_malloc(sizeof(t_seg));
	if(!new) 
	{
		free_seg_list(seg);
		return NULL;
	}
	new->val = malloc(sizeof(char) * len + 1);
	if(!new->val)
	{
		free(new);
		free_seg_list(seg);
		return NULL;
	}
	return new;
}

t_seg **add_segment(t_seg **seg, const char *val, size_t len, bool expand)
{
	t_seg *new;
	t_seg *temp;

	new = seg_init(seg, len);
	if(!new)
		return NULL;
	if(!ft_memcpy(new->val, val, len))
	{
		free_seg_list(seg);
		return NULL;
	}
	new->val[len] = 0;
	new->expand = expand; 
	new->next = NULL;
	if(!(*seg)) 
		*seg = new;
	else 
	{
		temp = *seg;
		while(temp->next)
			temp = temp->next;
		temp->next = new;
	}
	return seg;
}

t_token **add_token(t_token **tk, t_ttype type, t_seg *seg_list)
{ 
	t_token *new;
	t_token *temp;

	new = safe_malloc(sizeof(t_token));
	if(!new) {
		free_token_list(tk);
		return NULL; 
	}
	new->seg_list = seg_list;
	new->type = type;
	new->next = NULL;
	if(!(*tk)) 
		*tk = new;
	else 
	{
		temp = *tk;
		while(temp->next)
			temp = temp->next;
		temp->next = new;
	}
	return tk;
} 
