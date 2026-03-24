#include <lexer.h>

void print_token_list(t_token *tk)
{
    t_token *temp;
    t_seg *temp_seg;
    const char *arr[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "HEREDOC",
                         "APPEND", "UNCLOSED_QUOTE"};

    temp = tk;
    while (temp)
    {
        printf("%s\n", arr[temp->type]);
        temp_seg = temp->seg_list;
        while (temp_seg)
        {
            printf("(%s) exp(%s) has_quote(%s)\n", temp_seg->val,
                   temp_seg->expand ? "yes" : "no",
                   temp_seg->has_quote ? "yes" : "no");
            temp_seg = temp_seg->next;
        }
        temp = temp->next;
    }
}
