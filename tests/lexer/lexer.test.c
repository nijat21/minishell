#include "../../lexer.h"
#include <string.h>
#include "../../Libft/libft.h"

#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

t_token *lexer(const char *prompt);

typedef struct s_seg_test
{
	char *val;
	bool expand;
	bool has_quote;
} t_seg_test;

typedef struct s_token_test
{
	t_ttype type;
	t_seg_test *seg_arr;
	size_t seg_arr_len;
} t_token_test;

typedef struct s_test_case
{
	const char *input;
	t_token_test *expected;
	size_t expected_len;
} t_test_case;

size_t seg_len(t_seg *seg)
{
	size_t len;

	len = 0;
	while (seg)
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
	while (tk)
	{
		len++;
		tk = tk->next;
	}
	return len;
}

bool compare_tokens(t_token *actual, t_token_test *expected, size_t expected_len)
{
	size_t i = 0;
	while (actual && i < expected_len)
	{
		if (actual->type != expected[i].type)
			return false;

		t_seg *seg = actual->seg_list;
		if (expected[i].seg_arr_len > 0 && !seg)
			return false;

		for (size_t j = 0; j < expected[i].seg_arr_len; j++)
		{
			if (!seg)
				return false;
			if (strcmp(seg->val, expected[i].seg_arr[j].val) != 0)
				return false;
			if (seg->expand != expected[i].seg_arr[j].expand)
				return false;
			if (seg->has_quote != expected[i].seg_arr[j].has_quote)
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
void print_tokens(t_token *tokens)
{
	const char *arr[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND", "UNCLOSED_QUOTE"};

	while (tokens)
	{
		printf("(%s)", arr[tokens->type]);
		t_seg *seg = tokens->seg_list;
		while (seg)
		{
			printf(" %s (EXP: %s) (HAS_QUOTE: %s)", seg->val, seg->expand ? "yes" : "no", seg->has_quote ? "yes" : "no");
			seg = seg->next;
		}
		printf("\n");
		tokens = tokens->next;
	}
}

// Test runner
void run_tests(t_test_case *tests, size_t n)
{
	const char *arr[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND", "UNCLOSED_QUOTE"};
	size_t fail = 0;
	size_t pass = 0;

	for (size_t i = 0; i < n; i++)
	{
		bool ok; // = compare_tokens(result, tests[i].expected, tests[i].expected_len);
		// run each input 10 times
		t_token *result;
		size_t r_pc = 0;
		for (size_t j = 0; j < 10; j++)
		{
			result = lexer(tests[i].input);
			bool r_ok = compare_tokens(result, tests[i].expected, tests[i].expected_len);
			if (r_ok)
			{
				r_pc++;
				printf(GREEN "#" RESET);
			}
			else
				printf(RED "X" RESET);
		}
		printf("\n");
		ok = (r_pc == 10);
		if (ok)
		{
			printf(GREEN "Test %zu: PASS\n" RESET, i + 1);
			pass++;
		}
		else
		{
			fail++;
			printf(RED "Test %zu: FAIL\n" RESET, i + 1);
			printf(MAGENTA "  Input:    %s\n", tests[i].input);
			printf(BLUE "  Expected: \n");
			for (size_t j = 0; j < tests[i].expected_len; j++)
			{
				printf("(%s)", arr[tests[i].expected[j].type]);
				for (size_t k = 0; k < tests[i].expected[j].seg_arr_len; k++)
					printf(" %s (EXP: %s) (HAS_QUOTE: %s)", tests[i].expected[j].seg_arr[k].val,
						   tests[i].expected[j].seg_arr[k].expand ? "yes" : "no",
						   tests[i].expected[j].seg_arr[k].has_quote ? "yes" : "no");
				printf("\n");
			}
			printf(YELLOW "  Got:     \n");
			print_tokens(result);
			printf("\n" RESET);
		}
	}
	printf(MAGENTA "Summary: %zu passed, %zu failed\n" RESET, pass, fail);
}

int main()
{

	t_test_case tests[] = {
		{.input = "'", .expected = (t_token_test[]){{UNCLOSED_QUOTE, (t_seg_test[]){{"", false, true}}, 1}}, .expected_len = 1},
		{.input = "/", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"/", false, false}}, 1}}, .expected_len = 1},
		{.input = "\\", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"\\", false, false}}, 1}}, .expected_len = 1},
		{.input = "''", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"", false, true}}, 1}}, .expected_len = 1},
		{.input = "\"\"", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"", false, true}}, 1}}, .expected_len = 1},
		{.input = "hello", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, false}}, 1}}, .expected_len = 1},
		{.input = "hello world", .expected = (t_token_test[]){
									 {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									 {WORD, (t_seg_test[]){{"world", false, false}}, 1},
								 },
		 .expected_len = 2},
		{.input = "hello |", .expected = (t_token_test[]){
								 {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
								 {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
							 },
		 .expected_len = 2},
		{.input = "hello $", .expected = (t_token_test[]){
								 {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
								 {WORD, (t_seg_test[]){{"$", false, false}}, 1},
							 },
		 .expected_len = 2},
		{.input = "hello $?", .expected = (t_token_test[]){
								  {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
								  {WORD, (t_seg_test[]){{"$?", true, false}}, 1},
							  },
		 .expected_len = 2},
		{.input = "hello $?more", .expected = (t_token_test[]){
									  {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									  {WORD, (t_seg_test[]){
												 {"$?", true, false},
												 {"more", false, false},
											 },
									   2},
								  },
		 .expected_len = 2},
		{.input = "hello $_", .expected = (t_token_test[]){
								  {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
								  {WORD, (t_seg_test[]){{"$_", true, false}}, 1},
							  },
		 .expected_len = 2},
		{.input = "hello$", .expected = (t_token_test[]){
								{WORD, (t_seg_test[]){{"hello$", false, false}}, 1},
							},
		 .expected_len = 1},
		{.input = "$hello", .expected = (t_token_test[]){
								{WORD, (t_seg_test[]){{"$hello", true, false}}, 1},
							},
		 .expected_len = 1},
		{.input = "hello $var", .expected = (t_token_test[]){
									{WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									{WORD, (t_seg_test[]){{"$var", true, false}}, 1},
								},
		 .expected_len = 2},
		{.input = "hello $var'", .expected = (t_token_test[]){
									 {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									 {UNCLOSED_QUOTE, (t_seg_test[]){
														  {"$var", true, false},
														  {"", false, true},
													  },
									  2},
								 },
		 .expected_len = 2},
		{.input = "hello $var'else", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, false}}, 1}, {UNCLOSED_QUOTE, (t_seg_test[]){
																																		   {"$var", true, false},
																																		   {"else", false, true},
																																	   },
																													   2}},
		 .expected_len = 2},
		{.input = "hello$var'else", .expected = (t_token_test[]){{UNCLOSED_QUOTE, (t_seg_test[]){
																					  {"hello", false, false},
																					  {"$var", true, false},
																					  {"else", false, true},
																				  },
																  3}},
		 .expected_len = 1},
		{.input = "hello|", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, false}}, 1}, {PIPE, (t_seg_test[]){{"|", false, false}}, 1}}, .expected_len = 2},
		{.input = "\"hello\"", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, true}}, 1}}, .expected_len = 1},
		{.input = "\"", .expected = (t_token_test[]){{UNCLOSED_QUOTE, (t_seg_test[]){{"", false, true}}, 1}}, .expected_len = 1},
		{.input = "'", .expected = (t_token_test[]){{UNCLOSED_QUOTE, (t_seg_test[]){{"", false, true}}, 1}}, .expected_len = 1},
		{.input = "\"a", .expected = (t_token_test[]){{UNCLOSED_QUOTE, (t_seg_test[]){{"a", false, true}}, 1}}, .expected_len = 1},
		{.input = "'a", .expected = (t_token_test[]){{UNCLOSED_QUOTE, (t_seg_test[]){{"a", false, true}}, 1}}, .expected_len = 1},
		{.input = "hello \"", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, false}}, 1}, {UNCLOSED_QUOTE, (t_seg_test[]){{"", false, true}}, 1}}, .expected_len = 2},
		{.input = "hello '", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, false}}, 1}, {UNCLOSED_QUOTE, (t_seg_test[]){{"", false, true}}, 1}}, .expected_len = 2},
		{.input = "\"hello\"|", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, true}}, 1}, {PIPE, (t_seg_test[]){{"|", false, false}}, 1}}, .expected_len = 2},
		{.input = "\"hello\"world", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, true}, {"world", false, false}}, 2}}, .expected_len = 1},
		{.input = "\"hello\" world", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, true}}, 1}, {WORD, (t_seg_test[]){{"world", false, false}}, 1}}, .expected_len = 2},
		{.input = "some \"hello\"world", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"some", false, false}}, 1}, {WORD, (t_seg_test[]){{"hello", false, true}, {"world", false, false}}, 2}}, .expected_len = 2},
		{.input = "\"hello\" world", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, true}}, 1}, {WORD, (t_seg_test[]){{"world", false, false}}, 1}}, .expected_len = 2},
		{.input = "\"hello world", .expected = (t_token_test[]){
									   {UNCLOSED_QUOTE, (t_seg_test[]){{"hello world", false, true}}, 1},
								   },
		 .expected_len = 1},
		{.input = "\"hello 'world' test\"", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello 'world' test", false, true}}, 1}}, .expected_len = 1},
		{.input = "'hello \"world\" test'", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello \"world\" test", false, true}}, 1}}, .expected_len = 1},
		{.input = "hello > world", .expected = (t_token_test[]){
									   {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									   {REDIR_OUT, (t_seg_test[]){{">", false, false}}, 1},
									   {WORD, (t_seg_test[]){{"world", false, false}}, 1},
								   },
		 .expected_len = 3},
		{.input = "\"hello > test\"", .expected = (t_token_test[]){
										  {WORD, (t_seg_test[]){{"hello > test", false, true}}, 1},
									  },
		 .expected_len = 1},
		{.input = "\"\"hello", .expected = (t_token_test[]){
								   {WORD, (t_seg_test[]){{"", false, true}, {"hello", false, false}}, 2},
							   },
		 .expected_len = 1},
		{.input = "\"some$word\"", .expected = (t_token_test[]){
									   {WORD, (t_seg_test[]){{"some", false, true}, {"$word", true, true}}, 2},
								   },
		 .expected_len = 1},
		{.input = "'some$word'", .expected = (t_token_test[]){
									 {WORD, (t_seg_test[]){{"some$word", false, true}}, 1},
								 },
		 .expected_len = 1},
		{.input = "\"$hello\"", .expected = (t_token_test[]){
									{WORD, (t_seg_test[]){{"$hello", true, true}}, 1},
								},
		 .expected_len = 1},
		{.input = "'$hello'", .expected = (t_token_test[]){
								  {WORD, (t_seg_test[]){{"$hello", false, true}}, 1},
							  },
		 .expected_len = 1},
		{.input = "\"hello$\"", .expected = (t_token_test[]){
									{WORD, (t_seg_test[]){{"hello$", false, true}}, 1},
								},
		 .expected_len = 1},
		{.input = "'hello$'", .expected = (t_token_test[]){
								  {WORD, (t_seg_test[]){{"hello$", false, true}}, 1},
							  },
		 .expected_len = 1},
		{.input = "\"hello $\"", .expected = (t_token_test[]){
									 {WORD, (t_seg_test[]){{"hello $", false, true}}, 1},
								 },
		 .expected_len = 1},
		{.input = "\"hello $?more\"", .expected = (t_token_test[]){
										  {WORD, (t_seg_test[]){{"hello ", false, true}, {"$?", true, true}, {"more", false, true}}, 3},
									  },
		 .expected_len = 1},
		{.input = "'hello $?more'", .expected = (t_token_test[]){
										{WORD, (t_seg_test[]){{"hello $?more", false, true}}, 1},
									},
		 .expected_len = 1},
		{.input = "'hello $'", .expected = (t_token_test[]){
								   {WORD, (t_seg_test[]){{"hello $", false, true}}, 1},
							   },
		 .expected_len = 1},
		{.input = "\"some$word \"", .expected = (t_token_test[]){
										{WORD, (t_seg_test[]){
												   {"some", false, true},
												   {"$word", true, true},
												   {" ", false, true},
											   },
										 3},
									},
		 .expected_len = 1},
		{.input = "'some$word '", .expected = (t_token_test[]){
									  {WORD, (t_seg_test[]){{"some$word ", false, true}}, 1},
								  },
		 .expected_len = 1},
		{.input = "\"some $var'and\"", .expected = (t_token_test[]){
										   {WORD, (t_seg_test[]){{"some ", false, true}, {"$var", true, true}, {"'and", false, true}}, 3},
									   },
		 .expected_len = 1},
		{.input = "\"some $var'\"", .expected = (t_token_test[]){
										{WORD, (t_seg_test[]){{"some ", false, true}, {"$var", true, true}, {"'", false, true}}, 3},
									},
		 .expected_len = 1},
		{.input = "'some $var\"'", .expected = (t_token_test[]){
									   {WORD, (t_seg_test[]){{"some $var\"", false, true}}, 1},
								   },
		 .expected_len = 1},
		{.input = "hello|><", .expected = (t_token_test[]){
								  {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
								  {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
								  {REDIR_OUT, (t_seg_test[]){{">", false, false}}, 1},
								  {REDIR_IN, (t_seg_test[]){{"<", false, false}}, 1},
							  },
		 .expected_len = 4},
		{.input = "hello |><", .expected = (t_token_test[]){
								   {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
								   {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
								   {REDIR_OUT, (t_seg_test[]){{">", false, false}}, 1},
								   {REDIR_IN, (t_seg_test[]){{"<", false, false}}, 1},
							   },
		 .expected_len = 4},
		{.input = "hello |><<", .expected = (t_token_test[]){
									{WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									{PIPE, (t_seg_test[]){{"|", false, false}}, 1},
									{REDIR_OUT, (t_seg_test[]){{">", false, false}}, 1},
									{HEREDOC, (t_seg_test[]){{"<<", false, false}}, 1},
								},
		 .expected_len = 4},
		{.input = "hello |>><", .expected = (t_token_test[]){
									{WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									{PIPE, (t_seg_test[]){{"|", false, false}}, 1},
									{APPEND, (t_seg_test[]){{">>", false, false}}, 1},
									{REDIR_IN, (t_seg_test[]){{"<", false, false}}, 1},
								},
		 .expected_len = 4},
		{.input = "hello |>>< some", .expected = (t_token_test[]){
										 {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
										 {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
										 {APPEND, (t_seg_test[]){{">>", false, false}}, 1},
										 {REDIR_IN, (t_seg_test[]){{"<", false, false}}, 1},
										 {WORD, (t_seg_test[]){{"some", false, false}}, 1},
									 },
		 .expected_len = 5},
		{.input = "|>><some", .expected = (t_token_test[]){
								  {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
								  {APPEND, (t_seg_test[]){{">>", false, false}}, 1},
								  {REDIR_IN, (t_seg_test[]){{"<", false, false}}, 1},
								  {WORD, (t_seg_test[]){{"some", false, false}}, 1},
							  },
		 .expected_len = 4},
		{.input = "|>>< some", .expected = (t_token_test[]){
								   {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
								   {APPEND, (t_seg_test[]){{">>", false, false}}, 1},
								   {REDIR_IN, (t_seg_test[]){{"<", false, false}}, 1},
								   {WORD, (t_seg_test[]){{"some", false, false}}, 1},
							   },
		 .expected_len = 4},
		{.input = "\"hello|><\"", .expected = (t_token_test[]){
									  {WORD, (t_seg_test[]){{"hello|><", false, true}}, 1},
								  },
		 .expected_len = 1},
		{.input = "'hello|><'", .expected = (t_token_test[]){
									{WORD, (t_seg_test[]){{"hello|><", false, true}}, 1},
								},
		 .expected_len = 1},
		{.input = "a||b", .expected = (t_token_test[]){
							  {WORD, (t_seg_test[]){{"a", false, false}}, 1},
							  {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
							  {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
							  {WORD, (t_seg_test[]){{"b", false, false}}, 1},
						  },
		 .expected_len = 4},
		{.input = "", .expected = (t_token_test[]){}, .expected_len = 0},
		{.input = "   ", .expected = (t_token_test[]){}, .expected_len = 0},
		{.input = "   hello   ", .expected = (t_token_test[]){{WORD, (t_seg_test[]){{"hello", false, false}}, 1}}, .expected_len = 1},
		{.input = "hello   world", .expected = (t_token_test[]){
									   {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									   {WORD, (t_seg_test[]){{"world", false, false}}, 1},
								   },
		 .expected_len = 2},
		{.input = "a| |b", .expected = (t_token_test[]){
							   {WORD, (t_seg_test[]){{"a", false, false}}, 1},
							   {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
							   {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
							   {WORD, (t_seg_test[]){{"b", false, false}}, 1},
						   },
		 .expected_len = 4},
		{.input = ">>>", .expected = (t_token_test[]){
							 {APPEND, (t_seg_test[]){{">>", false, false}}, 1},
							 {REDIR_OUT, (t_seg_test[]){{">", false, false}}, 1},
						 },
		 .expected_len = 2},
		{.input = "<<>", .expected = (t_token_test[]){
							 {HEREDOC, (t_seg_test[]){{"<<", false, false}}, 1},
							 {REDIR_OUT, (t_seg_test[]){{">", false, false}}, 1},
						 },
		 .expected_len = 2},
		{.input = "<<EOF", .expected = (t_token_test[]){
							   {HEREDOC, (t_seg_test[]){{"<<", false, false}}, 1},
							   {WORD, (t_seg_test[]){{"EOF", false, false}}, 1},
						   },
		 .expected_len = 2},
		{.input = "<<'EOF'", .expected = (t_token_test[]){
								 {HEREDOC, (t_seg_test[]){{"<<", false, false}}, 1},
								 {WORD, (t_seg_test[]){{"EOF", false, true}}, 1},
							 },
		 .expected_len = 2},
		{.input = "<<\"EOF\"", .expected = (t_token_test[]){
								   {HEREDOC, (t_seg_test[]){{"<<", false, false}}, 1},
								   {WORD, (t_seg_test[]){{"EOF", false, true}}, 1},
							   },
		 .expected_len = 2},
		{.input = "<<", .expected = (t_token_test[]){
							{HEREDOC, (t_seg_test[]){{"<<", false, false}}, 1},
						},
		 .expected_len = 1},
		{.input = "pre$VARpost", .expected = (t_token_test[]){
									 {WORD, (t_seg_test[]){{"pre", false, false}, {"$VARpost", true, false}}, 2},
								 },
		 .expected_len = 1},
		{.input = "$VAR$VAR2", .expected = (t_token_test[]){
								   {WORD, (t_seg_test[]){{"$VAR", true, false}, {"$VAR2", true, false}}, 2},
							   },
		 .expected_len = 1},
		{.input = "a$?b", .expected = (t_token_test[]){
							  {WORD, (t_seg_test[]){{"a", false, false}, {"$?", true, false}, {"b", false, false}}, 3},
						  },
		 .expected_len = 1},
		{.input = "\"hello\"|\"world\"", .expected = (t_token_test[]){
											 {WORD, (t_seg_test[]){{"hello", false, true}}, 1},
											 {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
											 {WORD, (t_seg_test[]){{"world", false, true}}, 1},
										 },
		 .expected_len = 3},
		{.input = "echo \">\"", .expected = (t_token_test[]){
									{WORD, (t_seg_test[]){{"echo", false, false}}, 1},
									{WORD, (t_seg_test[]){{">", false, true}}, 1},
								},
		 .expected_len = 2},
		{.input = "echo '>'", .expected = (t_token_test[]){
								  {WORD, (t_seg_test[]){{"echo", false, false}}, 1},
								  {WORD, (t_seg_test[]){{">", false, true}}, 1},
							  },
		 .expected_len = 2},
		{.input = "\"|\"", .expected = (t_token_test[]){
							   {WORD, (t_seg_test[]){{"|", false, true}}, 1},
						   },
		 .expected_len = 1},
		{.input = "\"$VAR $VAR2\"", .expected = (t_token_test[]){
										{WORD, (t_seg_test[]){
												   {"$VAR", true, true},
												   {" ", false, true},
												   {"$VAR2", true, true},
											   },
										 3},
									},
		 .expected_len = 1},
		{.input = "'$VAR $VAR2'", .expected = (t_token_test[]){
									  {WORD, (t_seg_test[]){{"$VAR $VAR2", false, true}}, 1},
								  },
		 .expected_len = 1},
		{.input = "\"\"\"\"", .expected = (t_token_test[]){
								  {WORD, (t_seg_test[]){{"", false, true}, {"", false, true}}, 2},
							  },
		 .expected_len = 1},
		{.input = "\"a'b", .expected = (t_token_test[]){
							   {UNCLOSED_QUOTE, (t_seg_test[]){{"a'b", false, true}}, 1},
						   },
		 .expected_len = 1},
		{.input = "'a\"b", .expected = (t_token_test[]){
							   {UNCLOSED_QUOTE, (t_seg_test[]){{"a\"b", false, true}}, 1},
						   },
		 .expected_len = 1},
		{.input = "abc\"def", .expected = (t_token_test[]){
								  {UNCLOSED_QUOTE, (t_seg_test[]){{"abc", false, false}, {"def", false, true}}, 2},
							  },
		 .expected_len = 1},
		{.input = "hello\tworld", .expected = (t_token_test[]){
									  {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
									  {WORD, (t_seg_test[]){{"world", false, false}}, 1},
								  },
		 .expected_len = 2},
		{.input = "\thello\t|\tworld\t", .expected = (t_token_test[]){
											 {WORD, (t_seg_test[]){{"hello", false, false}}, 1},
											 {PIPE, (t_seg_test[]){{"|", false, false}}, 1},
											 {WORD, (t_seg_test[]){{"world", false, false}}, 1},
										 },
		 .expected_len = 3},
	};

	int n = sizeof(tests) / sizeof(t_test_case);
	run_tests(tests, n);
	return 0;
}
