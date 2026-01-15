#Minishell

## main concepts
    - prompt and input
    - Quoting
    - Expansion
    - tokenization
    - Redirection
    - Piping
    - Executions
    - Builtins


## parsing implementation
    - looped minishell prompt
    - reading input from terminal STDIN
        Quoting:
        - '' -> literal value of chars
        - "" -> literal value of chars except 
            - "$" and  "`"
            - "!" if history expansion is enabled
            - "\" if followed by one of "$, `, "", \" 
    - lexing the tokens
    - parsing tokens to some kind of a linked list
    
