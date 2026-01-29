#include "../../lexer.h"
#include <string.h>
#include "../../Libft/libft.h"

#define RESET       "\033[0m"
#define RED         "\033[1;31m" 
#define GREEN       "\033[1;32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

t_token* lexer(const char *prompt);

typedef struct s_seg_test {
	char *val;
	bool expand;
} t_seg_test;

typedef struct s_token_test {
	t_ttype type;
	t_seg_test *seg_arr;
	size_t seg_arr_len;
} t_token_test;

typedef struct s_test_case {
    const char *input;
    t_token_test *expected;
    size_t expected_len;
} t_test_case;

size_t seg_len(t_seg *seg)
{
	size_t len;

	len = 0;
	while(seg)
	{
		len++;
		seg = seg->next;
	}
	return len;
}

size_t token_list_len(t_token *tk)
{
	size_t len;

	len = 0;
	while(tk)
	{
		len++;
		tk = tk->next;
	}
	return len;
}

bool compare_tokens(t_token *actual, t_token_test *expected, size_t expected_len) {
	size_t i = 0;
	while (actual && i < expected_len) {
		if (actual->type != expected[i].type)
			return false;

		t_seg *seg = actual->seg_list;
		if (expected[i].seg_arr_len > 0 && !seg)
		    return false;
	
		for (size_t j = 0; j < expected[i].seg_arr_len; j++) {
		    if (!seg)
			return false;
		    if (strcmp(seg->val, expected[i].seg_arr[j].val) != 0)
			return false;
		    if (seg->expand != expected[i].seg_arr[j].expand)
			return false;
		    seg = seg->next;
		}
		if (seg != NULL) // actual has extra segments
		    return false;

		actual = actual->next;
		i++;
	}
	if (actual != NULL || i != expected_len)
		return false; // mismatch in number of tokens
	return true;
}

// Print token list for debug
void print_tokens(t_token *tokens) {
    	const char *arr[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND", "UNCLOSED_QUOTE"};

	while (tokens) {
		printf("(%s)", arr[tokens->type]);
		t_seg *seg = tokens->seg_list;
		while (seg) {
		    printf(" %s (EXP: %s)", seg->val, seg->expand ? "yes" : "no");
		    seg = seg->next;
        }
	printf("\n");	
        tokens = tokens->next;
    }
}

// Test runner
void run_tests(t_test_case *tests, size_t n) {
	const char *arr[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND", "UNCLOSED_QUOTE"};
	size_t fail = 0;
	size_t pass = 0;

	for (size_t i = 0; i < n; i++) {
		t_token *result = lexer(tests[i].input);
		bool ok = compare_tokens(result, tests[i].expected, tests[i].expected_len);

		if (ok) {
		    printf(GREEN"Test %zu: PASS\n"RESET, i + 1);
		    pass++;
		} else {
			fail++;	
		    printf(RED"Test %zu: FAIL\n"RESET, i + 1);
		    printf( MAGENTA"  Input:    %s\n", tests[i].input);
		    printf(BLUE"  Expected: \n");
		    for (size_t j = 0; j < tests[i].expected_len; j++) {
			printf("(%s)", arr[tests[i].expected[j].type]);
			for (size_t k = 0; k < tests[i].expected[j].seg_arr_len; k++)
			    printf(" %s (EXP: %s)", tests[i].expected[j].seg_arr[k].val,
				   tests[i].expected[j].seg_arr[k].expand ? "yes" : "no");
			printf("\n");
		    }
		    printf( YELLOW"  Got:     \n");
		    print_tokens(result);
		    printf("\n"RESET);
		}
	}
	printf(MAGENTA"Summary: %zu passed, %zu failed\n"RESET, pass, fail);
}


int main() {
	
	t_test_case tests[] = {
	    {
		.input = "hello",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 }
		},
		.expected_len = 1
	    },
	    {
		.input = "hello world",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { WORD, (t_seg_test[]){{"world", false}}, 1 },
		},
		.expected_len = 2
	    },
	    {
		.input = "hello |",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { PIPE, (t_seg_test[]){{"|", false}}, 1 }
		},
		.expected_len = 2
	    },
	    {
		.input = "hello $",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { WORD, (t_seg_test[]){{"$", false}}, 1 }
		},
		.expected_len = 2
	    },
	    {
		.input = "hello $?",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { WORD, (t_seg_test[]){{"$?", true}}, 1 }
		},
		.expected_len = 2
	    },
	    {
		.input = "hello $?more",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { WORD, (t_seg_test[]){{"$?", true}, {"more", false}}, 2}
		},
		.expected_len = 2
	    },
	    {
		.input = "hello $_",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { WORD, (t_seg_test[]){{"$_", true}}, 1 }
		},
		.expected_len = 2
	    },
	    {
		.input = "hello$",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello$", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "$hello",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"$hello", true}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		    .input = "hello $var",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { WORD, (t_seg_test[]){{"$var", true}}, 1 }
		    },
		    .expected_len = 2
	    },
	    {
		    .input = "hello $var'",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { UNCLOSED_QUOTE, (t_seg_test[]){
								    {"$var", true},
								    {"", false}
							    }, 2 },
		    },
		    .expected_len = 2
	    },
	    {
		    .input = "hello $var'else",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { UNCLOSED_QUOTE, (t_seg_test[]){
								    {"$var", true},
								    {"else", false},
							    }, 2 }
		    },
		    .expected_len = 2
	    },
	    {
		    .input = "hello$var'else",
		    .expected = (t_token_test[]){
			    { UNCLOSED_QUOTE, (t_seg_test[]){
								    {"hello", false}, 
								    {"$var", true},
								    {"else", false},
							    }, 3 }
		    },
		    .expected_len = 1
	    },
	    {
		.input = "hello|",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { PIPE, (t_seg_test[]){{"|", false}}, 1 }
		},
		.expected_len = 2
	    },
	    {
		    .input = "\"hello\"",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 }
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "\"",
		    .expected = (t_token_test[]){
			    { UNCLOSED_QUOTE, (t_seg_test[]){{"", false}}, 1 }
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "\"a",
		    .expected = (t_token_test[]){
			    { UNCLOSED_QUOTE, (t_seg_test[]){{"a", false}}, 1 }
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "hello \"",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { UNCLOSED_QUOTE, (t_seg_test[]){{"", false}}, 1 }
		    },
		    .expected_len = 2
	    },
	    {
		.input = "\"hello\"|",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { PIPE, (t_seg_test[]){{"|", false}}, 1 }
		},
		.expected_len = 2
	    },
	    {
		.input = "\"hello\"world",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}, {"world", false}}, 2 }
		},
		.expected_len = 1
	    },
	    {
		.input = "\"hello\" world",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { WORD, (t_seg_test[]){{"world", false}}, 1 }
		},
		.expected_len = 2
	    },
	    {
		.input = "some \"hello\"world",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"some", false}}, 1 },
		    { WORD, (t_seg_test[]){{"hello", false}, {"world", false}}, 2 }
		},
		.expected_len = 2
	    },
	    {
		.input = "\"hello\" world",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { WORD, (t_seg_test[]){{"world", false}}, 1 }
		},
		.expected_len = 2
	    },
	    {
		    .input = "\"hello world",
		    .expected = (t_token_test[]){
			    { UNCLOSED_QUOTE, (t_seg_test[]){{"hello world", false}}, 1 },
		    },
		    .expected_len = 1
	    },
	    {
		.input = "\"hello 'world' test\"",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello 'world' test", false}}, 1 }
		},
		.expected_len = 1
	    },
	    {
		.input = "'hello \"world\" test'",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello \"world\" test", false}}, 1 }
		},
		.expected_len = 1
	    },
	    {
		.input = "hello > world",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		    { REDIR_OUT, (t_seg_test[]){{">", false}}, 1 },
		    { WORD, (t_seg_test[]){{"world", false}}, 1 },
		},
		.expected_len = 3
	    },
	    {
		.input = "\"hello > test\"",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello > test", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "\"\"hello",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "\"some$word\"",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"some", false}, {"$word", true}}, 2 },
		},
		.expected_len = 1
	    },
	    {
		.input = "'some$word'",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"some$word", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "\"$hello\"",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"$hello", true}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "'$hello'",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"$hello", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "\"hello$\"",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello$", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "'hello$'",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello$", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "\"hello $\"",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello $", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "\"hello $?more\"",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){
						  {"hello ", false},
						  {"$?", true},
						  {"more", false}
					  }, 3 },
		},
		.expected_len = 1
	    },
	    {
		.input = "'hello $?more'",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello $?more", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		.input = "'hello $'",
		.expected = (t_token_test[]){
		    { WORD, (t_seg_test[]){{"hello $", false}}, 1 },
		},
		.expected_len = 1
	    },
	    {
		    .input = "\"some$word \"",
		    .expected = (t_token_test[]){
			{ WORD, (t_seg_test[])
				{
					{"some", false}, 
					{"$word", true}, 
					{" ", false}, 
				},
			3 },
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "'some$word '",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"some$word ", false}}, 1 },
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "\"some $var'and\"",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){
							  {"some ", false},
							  {"$var", true},
							  {"'and", false}
						  }, 3 },
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "\"some $var'\"",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){
							  {"some ", false},
							  {"$var", true},
							  {"'", false}
						  }, 3 },
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "'some $var\"'",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"some $var\"", false}}, 1 },
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "hello|><",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { PIPE, (t_seg_test[]){{"|", false}}, 1 },
			    { REDIR_OUT, (t_seg_test[]){{">", false}}, 1 },
			    { REDIR_IN, (t_seg_test[]){{"<", false}}, 1 },
		    },
		    .expected_len = 4
	    },
	    {
		    .input = "hello |><",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { PIPE, (t_seg_test[]){{"|", false}}, 1 },
			    { REDIR_OUT, (t_seg_test[]){{">", false}}, 1 },
			    { REDIR_IN, (t_seg_test[]){{"<", false}}, 1 },
		    },
		    .expected_len = 4
	    },
	    {
		    .input = "hello |><<",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { PIPE, (t_seg_test[]){{"|", false}}, 1 },
			    { REDIR_OUT, (t_seg_test[]){{">", false}}, 1 },
			    { HEREDOC, (t_seg_test[]){{"<<", false}}, 1 },
		    },
		    .expected_len = 4
	    },
	    {
		    .input = "hello |>><",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { PIPE, (t_seg_test[]){{"|", false}}, 1 },
			    { APPEND, (t_seg_test[]){{">>", false}}, 1 },
			    { REDIR_IN, (t_seg_test[]){{"<", false}}, 1 },
		    },
		    .expected_len = 4
	    },
	    {
		    .input = "hello |>>< some",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello", false}}, 1 },
			    { PIPE, (t_seg_test[]){{"|", false}}, 1 },
			    { APPEND, (t_seg_test[]){{">>", false}}, 1 },
			    { REDIR_IN, (t_seg_test[]){{"<", false}}, 1 },
			    { WORD, (t_seg_test[]){{"some", false}}, 1 },
		    },
		    .expected_len = 5
	    },
	    {
		    .input = "|>><some",
		    .expected = (t_token_test[]){
			    { PIPE, (t_seg_test[]){{"|", false}}, 1 },
			    { APPEND, (t_seg_test[]){{">>", false}}, 1 },
			    { REDIR_IN, (t_seg_test[]){{"<", false}}, 1 },
			    { WORD, (t_seg_test[]){{"some", false}}, 1 },
		    },
		    .expected_len = 4
	    },
	    {
		    .input = "|>>< some",
		    .expected = (t_token_test[]){
			    { PIPE, (t_seg_test[]){{"|", false}}, 1 },
			    { APPEND, (t_seg_test[]){{">>", false}}, 1 },
			    { REDIR_IN, (t_seg_test[]){{"<", false}}, 1 },
			    { WORD, (t_seg_test[]){{"some", false}}, 1 },
		    },
		    .expected_len = 4
	    },
	    {
		    .input = "\"hello|><\"",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello|><", false}}, 1 },
		    },
		    .expected_len = 1
	    },
	    {
		    .input = "'hello|><'",
		    .expected = (t_token_test[]){
			    { WORD, (t_seg_test[]){{"hello|><", false}}, 1 },
		    },
		    .expected_len = 1
	    },

	};
	

    
    int n = sizeof(tests) / sizeof(t_test_case);
    run_tests(tests, n);
    return 0;
}

