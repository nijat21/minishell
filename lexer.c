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

/* void add_token(t_token **tk, t_ttype type, char *val_start, size_t len, bool expandable) */
/* { */ 
/* 	t_token *new; */
  
/* 	new = malloc(sizeof(t_token*)); */
/* 	new->type = type; */
/* 	new->value = val; */
/* 	new->expandable = expandable; */ 
/* 	if(!(*tk)) *tk = new; */
/* } */   

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
   
char *slice(char *str, size_t len)
{
	size_t i;
	char *substr;

	substr = safe_malloc(sizeof(char) * len);
	if(!substr) 
		return NULL;
	i = -1;
	while(str[++i] && i < len)
		substr[i] = str[i];
	return substr;
}

void nullify(char *str)
{
	while(*str)
	{
		*str = 0;
		str++;
	}
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

/*
 	* When to add a token?
	*
	* word -> done 
	* "word" -> 
	* "word"something
	* some "word"something
	* "word" something
	* "word 'something' word "
	* 'word "something" word '	
*/


void prompt_to_list(const char *prompt)
{ 
 	/* t_token *tk; */
	t_quote qc;
 	int i;
	int j;
	char token[10000];
   
	/* tk = NULL; */
	qc = Q_NONE;
	i = -1;
	j = 0;
 	while(prompt[++i])
 	{ 
		quote_context(prompt[i], &qc);
		if(qc == Q_NONE) // in quotes
		{ 		
			if(is_space(prompt[i]))
			{ 	// create the token ;
				printf("Token space -> %s\n", token);
				j = 0;
				nullify(token);
				continue;
			}
			// consider the operators
			// Pipe, redirs, heredoc 
			printf("none -> %c\n", prompt[i]);
			token[j] = prompt[i];
			token[j+1] = 0;
			j++;
		}
		else if(qc == Q_SINGLE || qc == Q_DOUBLE)
		{
			while(prompt[++i] && qc != Q_NONE)
			{
				if((qc == Q_SINGLE && prompt[i] == '\'') || 
				   (qc == Q_DOUBLE && prompt[i] == '"'))
					continue;
				quote_context(prompt[i], &qc);
				token[j] = prompt[i];	
				token[j+1] = 0;
				j++;
			}
		}
 	}
	token[j] = 0;
	printf("Token last -> %s\n", token);
	nullify(token);
}

/* void prompt_to_list(const char *prompt) */
/* { */ 
/* 	t_token *tk; */
/* 	int i; */
/* 	int start; */
/* 	bool exp; */
/* 	t_ttype token_type; */
/* 	char *pair_found; */
/* 	char *temp; */

/* 	tk = NULL; */ 

/* 	i = -1; */
/* 	while(is_space(prompt[++i])) ; */
/* 	exp = true; */
/* 	while(prompt[++i]) */
/* 	{ */ 
/* 		if(is_quote(prompt[i])) */ 
/* 		{ */ 
/* 			if(prompt[i] == '\'') */ 
/* 				exp=false; */
/* 			i++; */ 
/* 			start = i; */  
/* 			pair_found = ft_strchr(&prompt[i], prompt[i]); */
/* 			if(!pair_found) */   
/* 			{ */     
/* 				/1* add_token(tk, unclosed_quote, "", 0, exp); *1/ */
/* 				printf("Token -> %s type -> %s exp -> %b\n", "", UNCLOSED_QUOTE, exp); */
/* 				break; */   
/* 			} */   
/* 			/1* else copy until the closing quotation mark *1/ */
/* 			while(prompt[i] && (&prompt[i] <= pair_found)) */
/* 				i++; */ 
/* 			if(!prompt[i] || ( prompt[i] && is_space(prompt[i]) )) */
/* 			{ */ 	
/* 				/1* add_token(tk, WORD, &prompt[start], i - start, exp); *1/ */ 
/* 				printf("Token -> %s type -> %s exp -> %b\n", slice(&prompt[start], i - start), UNCLOSED_QUOTE, exp); */
/* 				exp=true; */	
/* 			} */
/* 			/1* and if there's no space after closing quote continue *1/ */
/* 		} */
/* 		else if(is_operator(prompt[i])) */ 
/* 			// Pipe, redirs, heredoc */ 
/* 		else */ 
/* 	} */
/* 	/1* add_token(tk, token_type, &prompt[i], i - start, exp); *1/ */
/* 	printf("Token -> %s type -> %s exp -> %b\n", slice(&prompt[start], i - start), UNCLOSED_QUOTE, exp); */
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
