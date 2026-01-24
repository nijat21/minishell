#include "lexer.h"
#include "Libft/libft.h"

/*
 	TOKENISATION ->

	Detect operators |, <, >, <<, >>
	Detect quotes '', "" (ignore unclosed quotes)
	Unsopported chars '; and  \' 

	>echo "tell 'me' "something" "
	tell 'me' something

	>echo 'tell "me" 'something' '
	tell "me" something
	
	>echo 'tell 'me' something'
	tell me something
	
	>echo "tell "me" something "
	tell me something
	
	>echo some"think$hello" -> WORD(echo) WORD(some"think$hello")
	somethinkHELLO
*/

bool	is_operator(const char c)
{
	return (c == '|' || c == '<' || c == '>');
}


bool	is_quote(const char c)
{
	return (c == '\'' || c == '"');
}

void quote_context(const char c, t_quote *qc)
{
	if(*qc == Q_NONE)
	{ 
		if(c == '\'') 
			*qc = Q_SINGLE;
		else if(c == '"')
			*qc = Q_DOUBLE;
	}
	else if (*qc == Q_SINGLE)
	{
		if(c == '\'') 
			*qc = Q_NONE;
	}
	else if (*qc == Q_DOUBLE)
	{
		if(c == '"') 
			*qc = Q_NONE;
	}
}

void choose_ttype(const char *str, t_ttype *tt)
{
	if(*tt == UNCLOSED_QUOTE)
		return;
	if(*str == '|')
		*tt = PIPE;
	else if(*str == '<')
	{ 	
		str++;
		if(*str && *str == '<')
			*tt = HEREDOC;
		else
			*tt = REDIR_IN; 
	}
	else if(*str == '>')
	{
		str++;
		if(*str && *str == '>')
			*tt = APPEND;
		else
			*tt = REDIR_OUT; 
	}
	else 
		*tt = WORD;
}

void add_token(t_token **tk, t_ttype type, t_seg *seg_list)
{ 
	t_token *new;
	t_token *temp;

	new = safe_malloc(sizeof(t_token));
	if(!new) {
		free_token_list(*tk);
		return; 
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
}   

void add_segment(t_seg **seg, const char *val, size_t len, bool expand)
{
	t_seg *new;
	t_seg *temp;

	new = safe_malloc(sizeof(t_seg));
	if(!new) 
	{
		free_seg_list(*seg);
		return ;
	}
	new->val = malloc(sizeof(char) * len + 1);
	if(!new->val)
	{
		free_seg_list(*seg);
		return ;
	}
	if(!ft_memcpy(new->val, val, len))
	{
		free_seg_list(*seg);
		return ;
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
}

/*
 * Check list:
 *
 * word  
 * word |
 * word|
 * word "
 * "word"  
 * "word"|  
 * "word"something  
 * "word" something  
 * some "word"something 
 * "word" something
 * "word 'something' word "
 * 'word "something" word '	
 */

// In quotes
	// '' / ' "" ' -> exp=false, type=WORD, 1 seg all text
	// "" / " '' "	
		// $ -> exp=false/true, type=WORD, 2 seg split text
		// exp=false, type=WORD, 1 seg all text
	// '/" -> exp=false, type=UNCLOSED_QUOTE, 1 seg 
// Out quotes
	// $ -> exp=true, type=WORD, 1 seg per str, 2 if str has $ in middle
	// exp=false, type=WORD, 1 seg per str
	// operator -> exp=false, type=(oper), 1 seg per operator


void lexer(t_token **tk, const char *str)
{
	t_quote qc;
	/* t_token *tk; */
	t_seg *seg;
	size_t len;
	bool exp;
	t_ttype tt;
	int i;
	const char *start;

	const char *arr[] = {"Q_NONE", "Q_SINGLE", "Q_DOUBLE"};
	
	qc = Q_NONE;
	*tk = NULL;
	seg = NULL;
	exp = false;
	len = 0;
	i = -1;
	while(str[++i])
	{
		quote_context(*str, &qc);
		printf("qc -> %s len -> %ld\n", arr[qc], len);
		if(is_space(str[i]))
			continue;
		if(qc == Q_SINGLE)
		{
			exp = false;
			while(str[++i] && qc != Q_NONE)
			{
				quote_context(str[i], &qc);
				if(qc == Q_NONE) 
				{
					if(len > 0)
					{ 	
						tt = WORD;
						add_segment(&seg, start, len, exp);
						len = 0;
					}
					break; 
				}
				if(len == 0)
					start = &str[i];
				len++;
			}
			if(!str[i] && qc == Q_SINGLE)
			{ 
				add_segment(&seg, start, len, exp);
				add_token(tk, UNCLOSED_QUOTE, seg);
				len = 0;
				seg = NULL;
			}			
		}
		else if(qc == Q_DOUBLE)
		{
			exp = false;
			tt = WORD;
			while(str[++i] && qc != Q_NONE)
			{
				quote_context(str[i], &qc);
				/* printf("qc -> %s len -> %ld\n", arr[qc], len); */
				if(qc == Q_NONE) 
				{
					if(len > 0)
					{ 	
						tt = WORD;
						add_segment(&seg, start, len, exp);
						len = 0;
						exp = false;
					}
					if(qc == Q_NONE)
						break; 
				}
				if(str[i] == '$' && (str[i+1])) // if $ -is last char, it's char literal, exp=false // handle $?
					exp = true;
				if(len == 0)
					start = &str[i];
				len++;
			}
			if(!str[i] && qc == Q_DOUBLE)
			{ 
				add_segment(&seg, start, len, exp);
				add_token(tk, UNCLOSED_QUOTE, seg);
				len = 0;
				seg = NULL;
			}			
		}
		else
		{
			/* printf("tt -> %s char -> %c\n", arr1[tt], str[i]); */
			if(len == 0)
				start = &str[i];
			if(str[i] == '$')
				exp = true;
			if(is_space(str[i]))
			{
				if(len > 0)
				{ 
					add_segment(&seg, start, len, exp);
					add_token(tk, WORD, seg);
					len = 0;
					seg = NULL;
					exp = false;
				}
				else 
					continue;
			}
			else if(is_operator(str[i]))
			{
				if(len > 0)
				{
					add_segment(&seg, start, len, exp);
					add_token(tk, WORD, seg);
					len = 0;
					seg = NULL;
					exp = false;
				}
				choose_ttype(&str[i], &tt);
				start = &str[i];
				len++;
				if(tt == HEREDOC || tt == APPEND)
				{
					len++;
					i++;
				}
				add_segment(&seg, start, len, exp);
				add_token(tk, tt, seg);
				len = 0;
				seg = NULL;
				exp = false;
				tt = WORD;
			}
			else
				len++;
		}
	}
	if(len > 0)
		add_segment(&seg, start, len, exp);
	if(seg)
		add_token(tk, tt, seg);
}

void *tokenise(void)
{
	char *prompt;
	t_token *tk;

    	while (1)
	{
		write(1, "minishell> ", 11);
		prompt = readline("");
		if(!prompt)
		{
			perror("Read");
			return NULL;
		}
		// printf("Read -> %s\n", prompt);

		lexer(&tk, prompt);
		if(!tk)
			printf("Include proper input\n");
		print_token_list(tk);
	}
	return prompt;
}
