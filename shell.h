#ifndef SHELL_H
#define SHELL_H
#define MAX_HISTORY 100
#define MAX_TOKENS 128
#define MAX_CMD_LENGTH 1024

typedef enum {
    TOKEN_PIPE,
    TOKEN_WORD,
    TOKEN_SEMICOLON,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct Command {
    char **args;
    struct Command *next;    
    struct Command *pipe_to; 
} Command;


Token *tokenize_input(const char *input, int *token_count);
Command *parse_tokens(Token *tokens, int token_count);
void execute_command_chain(Command *cmd);
void free_command_chain(Command *cmd);

#endif
