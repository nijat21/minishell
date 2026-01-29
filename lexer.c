#include "lexer.h"

/*
 	TOKENISATION ->

	Detect operators |, <, >, <<, >>
	Detect quotes '', "" (ignore unclosed quotes)
	Unsopported chars '; and  \'
	$?, $var segs exp=true unless '$var'
*/
  

t_token *lexer(const char *str)
{
	t_quote qc;
	t_token *tk;
	t_seg *seg;
	size_t len;
	t_ttype tt;
	int i;
	const char *start;
	size_t var_len;

	tk = NULL;
	qc = Q_NONE;
	seg = NULL;
	len = 0;
	i = -1;
	while(str[++i])
	{
		quote_context(str[i], &qc);
		choose_ttype(&str[i], &tt);
		if(qc == Q_DOUBLE || qc == Q_SINGLE)
		{
			if(len > 0)
			{
				add_segment(&seg, start, len, false);
				len = 0;
			}
			while(str[++i] && qc != Q_NONE)
			{
				quote_context(str[i], &qc);
				if(qc == Q_NONE) 
				{
					if(len > 0)
					{ 	
						add_segment(&seg, start, len, false);
						len = 0;
					}
					break; 
				}
				if(qc == Q_DOUBLE && str[i] == '$')
				{
					var_len = varname_len(&str[i]);
					if(var_len)
					{ 
						if(len > 0)
						{ 	
							add_segment(&seg, start, len, false);
							len = 0;
						}
						add_segment(&seg, &str[i], var_len, true);
						i += var_len - 1;
						continue;
					}
				}
				if(len == 0)
					start = &str[i];
				len++;
			}
			if( qc != Q_NONE)
			{ 	
				tt = UNCLOSED_QUOTE; 
				break;
			}
		}
		else
		{
			if(len == 0)
				start = &str[i];
			if(str[i] == '$')
			{
				var_len = varname_len(&str[i]);
				if(var_len)
				{ 
					if(len > 0)
					{ 	
						add_segment(&seg, start, len, false);
						len = 0;
					}
					add_segment(&seg, &str[i], var_len, true);
					i += var_len - 1;
					continue;
				}
			}
			if(is_space(str[i]))
			{
				if(len == 0 && seg)
				{
					add_token(&tk, WORD, seg);
					seg = NULL;
				}				
				if(len > 0)
				{ 
					add_segment(&seg, start, len, false);
					add_token(&tk, WORD, seg);
					len = 0;
					seg = NULL;
				}
				else 
					continue;
			}
			else if(is_operator(str[i]))
			{
				if(len == 0 && seg)
				{
					add_token(&tk, WORD, seg);
					seg = NULL;
				}				
				if(len > 0)
				{
					add_segment(&seg, start, len, false);
					add_token(&tk, WORD, seg);
					len = 0;
					seg = NULL;
				}
				start = &str[i];
				len++;
				if(tt == HEREDOC || tt == APPEND)
				{
					len++;
					i++;
				}
				add_segment(&seg, start, len, false);
				add_token(&tk, tt, seg);
				len = 0;
				seg = NULL;
			}
			else
				len++;
		}
	}
	if(len > 0 || tt == UNCLOSED_QUOTE)
		add_segment(&seg, start, len, false);
	if(seg)
		add_token(&tk, tt, seg);
	return tk;
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
		tk = lexer(prompt);
		if(!tk)
			printf("Include proper input\n");
		print_token_list(tk);
	}
	return prompt;
}
