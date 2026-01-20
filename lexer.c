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

static int	is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v'
		|| c == '\f')
		return (1);
	else
		return (0);
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

void* add_token(t_token **tk, t_ttype type, t_seg *seg_list)
{ 
	t_token *new;
	t_token *temp;

	new = safe_malloc(sizeof(t_token));
	if(!new) {
		free_token_list(*tk);
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
	return *tk;
}   

void* add_segment(t_seg **seg, const char *val, size_t len, bool expand)
{
	t_seg *new;
	t_seg *temp;

	new = safe_malloc(sizeof(t_seg));
	if(!new) 
	{
		free_seg_list(*seg);
		return NULL;
	}
	new->val = malloc(sizeof(char) * len);
	if(!new->val)
	{
		free_seg_list(*seg);
		return NULL;
	}
	if(!ft_memcpy(new->val, val, len))
	{
		free_seg_list(*seg);
		return NULL;
	}
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
	return *seg; 
}

/*
 * Check list:
 *
 * word  
 * word |
 * word|
 * "word"  
 * "word"|  
 * "word"something  
 * "word" something  
 * some "word"something 
 * "word" something
 * "word 'something' word "
 * 'word "something" word '	
 */

void print_seg_list(t_seg *seg);
void print_token_list(t_token *tk);

// I am using same seg variable to add to several tokens without seg=NULL
// I am pushing every segment to a token. What if token is "hello"world, 'hello'world.
// I am not using the correct extendable value becuase it relies on quote context
t_token* prompt_to_list(const char *prompt)
{ 
 	t_token *tk;
	t_seg *seg_list;
	t_quote qc;
	t_ttype tt;

 	int i;
	size_t len;
	const char *start;

	bool exp;
   
	tk = NULL;
	seg_list = NULL;

	qc = Q_NONE;
	i = -1;
	len = 0;
	exp = true;
	// These should be one token two segments -> "hello"world, 'hello'world
 	while(prompt[++i])
 	{ 
		quote_context(prompt[i], &qc);
		choose_ttype(&prompt[i], &tt);
		if(qc == Q_NONE) 
		{ 		
			if(tt != WORD)
			{
				if(seg_list)
				{ 	
					add_token(&tk, tt, seg_list);
					seg_list = NULL;
				}
				else if(!seg_list && len > 0) 
				{
					add_segment(&seg_list, start, len, exp);
					add_token(&tk, tt, seg_list);
					seg_list = NULL;
					len = 0;
				}
				start = &prompt[i];
				exp = true;
				if(tt == HEREDOC || tt == APPEND)
				{ 
					len = 2; 
					i++;
				}
				else 
					len = 1;
				add_segment(&seg_list, start, len, exp);
				add_token(&tk, tt, seg_list);
				seg_list = NULL;
				len = 0;
				continue;
			}
			if(is_space(prompt[i]))
			{
				if(seg_list)
				{ 	
					add_token(&tk, tt, seg_list);
					seg_list = NULL;
				}
				else if(!seg_list && len > 0)
				{
					add_segment(&seg_list, start, len, exp);
					add_token(&tk, tt, seg_list);
					seg_list = NULL;
					len = 0;
				}
				continue; 
			}  
			if(len == 0)   
			{ 	
				start = &prompt[i];
				exp = true;
			}
			len++;
		}
		else if(qc == Q_SINGLE || qc == Q_DOUBLE)
		{
			while(prompt[++i] && qc != Q_NONE)
			{
				quote_context(prompt[i], &qc);
				if(qc == Q_NONE) 
				{ 	
					if(len > 0)
					{
						add_segment(&seg_list, start, len, exp);
						len = 0;
					}
					break;
				}
				if(len == 0)
				{ 	
					start = &prompt[i];
					exp = true;
					if(qc == Q_SINGLE)
						exp = false;
				}
				len++;
			}
			if(!prompt[i] && (qc != Q_NONE))
				tt = UNCLOSED_QUOTE;
		}
 	}
	if(seg_list)
		add_token(&tk, tt, seg_list);
	else if(!seg_list && len > 0)
	{ 	
		add_segment(&seg_list, start, len, exp);
		add_token(&tk, tt, seg_list);
	}
	seg_list = NULL;
	return tk;
}

const char *ttype_to_str(t_ttype t)
{
    static const char *map[] = {
        "WORD",
        "PIPE",
        "REDIR_IN",
        "REDIR_OUT",
        "HEREDOC",
        "APPEND",
        "UNCLOSED_QUOTE"
    };

    if (t < 0 || t >= (int)(sizeof(map)/sizeof(map[0])))
        return "UNKNOWN";

    return map[t];
}

void print_seg_list(t_seg *seg)
{
	t_seg *temp;

	temp = seg;
	while(temp)
	{
		printf("seg val -> %s\n", temp->val);
		printf("seg exp -> %d\n", temp->expand);
		temp = temp->next;
	}
}

void print_token_list(t_token *tk)
{
	t_token *temp;

	temp = tk;
	while(temp)
	{
		printf("tk type -> %s\n", ttype_to_str(tk->type));
		print_seg_list(temp->seg_list);
		temp = temp->next;
	}
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

		tk = prompt_to_list(prompt);
		if(!tk)
			printf("done\n");

		print_token_list(tk);
		//ft_memcpy(buf,&buf[by_read], BUFFER_SIZE - by_read);
	}
	return prompt;
}
