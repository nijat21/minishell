#include "lexer.h"

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


void append_token(t_token **tk, char *str, size_t len, t_ttype token_type, bool expand)
{ 
	t_token *new;
	t_token *temp;

	new = safe_malloc(sizeof(t_token*));
	if(!new) {
		free_list(*tk);
		return;
	}
	new->value = malloc(sizeof(char) * len);
	if(!new->value)
	{
		free_list(*tk);
		return;
	}
	if(!ft_memcpy(new->val, str, len))
	{
		free_list(*tk);
		return;
	}
	new->type = type;
	new->expand = expand; 
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

/*
 * When to add a token?
 *
 * word -> done 
 * "word" -> done 
 * "word"something -> done 
 * "word" something -> done 
 * some "word"something -> done
 * "word" something
 * "word 'something' word "
 * 'word "something" word '	
 */

void prompt_to_list(const char *prompt)
{ 
 	t_token *tk;
	t_quote qc;
 	int i;
	size_t len;
	char *start;
   
	tk = NULL;
	qc = Q_NONE;
	i = -1;
	len = 0;
 	while(prompt[++i])
 	{ 
		quote_context(prompt[i], &qc);
		if(qc == Q_NONE) 
		{ 		
			if(is_space(prompt[i]))
				printf("Token space -> %s\n", token);
				if(len > 0)
				{
					// flush token
					/* append_token(tk, start, len, WORD, ) */
					len = 0;
				}
				continue;
			}
			if(len == 0) 
				start = &prompt[i];
			// consider the operators
			// Pipe, redirs, heredoc 
			printf("copied in word -> %c\n", prompt[i]);
			len++;
		}
		else if(qc == Q_SINGLE || qc == Q_DOUBLE)
		{
			while(prompt[++i] && qc != Q_NONE)
			{
				quote_context(prompt[i], &qc);
				if(qc == Q_NONE) 
					break;
				if(len == 0)
					start = &prompt[i];
				// else
				// append to token
				printf("copied in quotes -> %c\n", prompt[i]);
				len++;
			}
		}
 	}
	// flush token
	printf("Token last -> %s\n", token);
}

/* void prompt_to_list(const char *prompt) */
/* { */ 
/*  	/1* t_token *tk; *1/ */
/* 	t_quote qc; */
/*  	int i; */
/* 	int j; */
/* 	size_t len; */
/* 	char token[10000]; */
   
/* 	/1* tk = NULL; *1/ */
/* 	qc = Q_NONE; */
/* 	i = -1; */
/* 	j = -1; */
/* 	len = 0; */
/*  	while(prompt[++i]) */
/*  	{ */ 
/* 		quote_context(prompt[i], &qc); */
/* 		if(qc == Q_NONE) */ 
/* 		{ */ 		
/* 			if(is_space(prompt[i])) */
/* 				if(!token[0]) */ 
/* 					continue; */ 
/* 				// flush token */
/* 				printf("Token space -> %s\n", token); */
/* 				j = -1; */
/* 				len = 0; */
/* 				nullify(token); */
/* 				continue; */
/* 			} */
/* 			// consider the operators */
/* 			// Pipe, redirs, heredoc */ 
/* 			printf("copied in word -> %c\n", prompt[i]); */
/* 			token[++j] = prompt[i]; */
/* 			token[j+1] = 0; */
/* 			len++; */
/* 		} */
/* 		else if(qc == Q_SINGLE || qc == Q_DOUBLE) */
/* 		{ */
/* 			while(prompt[++i] && qc != Q_NONE) */
/* 			{ */
/* 				quote_context(prompt[i], &qc); */
/* 				if(qc == Q_NONE) */ 
/* 					break; */
/* 				if((qc == Q_SINGLE && prompt[i] == '\'') || */ 
/* 				   (qc == Q_DOUBLE && prompt[i] == '"')) */
/* 					continue; */
/* 				// append to token */
/* 				printf("copied in quotes -> %c\n", prompt[i]); */
/* 				token[++j] = prompt[i]; */	
/* 				token[j+1] = 0; */
/* 			} */
/* 		} */
/*  	} */
/* 	// flush token */
/* 	printf("Token last -> %s\n", token); */
/* 	nullify(token); */
/* } */

void *tokenise(void)
{
	char *prompt;

    	while (1)
	{
		write(1, "minishell> ", 11);
		prompt = readline("");
		if(!prompt)
		{
			perror("Read");
			return NULL;
		}
		// here: normally parse + execute
		// for now, do nothing

		// printf("Read -> %s\n", prompt);

		prompt_to_list(prompt);	

		//ft_memcpy(buf,&buf[by_read], BUFFER_SIZE - by_read);
	}
	return prompt;
}
