#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

Command *parse_tokens(Token *tokens, int token_count) {
    Command *head = NULL, *current = NULL;
    int i = 0;

    while (i < token_count && tokens[i].type != TOKEN_EOF) {
        Command *cmd = malloc(sizeof(Command));
        cmd->args = malloc(sizeof(char *) * MAX_TOKENS);
        cmd->next = NULL;
        cmd->pipe_to = NULL;

        int argc = 0;
        while (i < token_count && tokens[i].type == TOKEN_WORD) {
            cmd->args[argc++] = strdup(tokens[i].value);
            i++;
        }
        cmd->args[argc] = NULL;

        Command *last_pipe = cmd;

        while (i < token_count && tokens[i].type == TOKEN_PIPE) {
            i++; // skip '|'

            Command *pipe_cmd = malloc(sizeof(Command));
            pipe_cmd->args = malloc(sizeof(char *) * MAX_TOKENS);
            pipe_cmd->next = NULL;
            pipe_cmd->pipe_to = NULL;

            int pargc = 0;
            while (i < token_count && tokens[i].type == TOKEN_WORD) {
                pipe_cmd->args[pargc++] = strdup(tokens[i].value);
                i++;
            }
            pipe_cmd->args[pargc] = NULL;

            last_pipe->pipe_to = pipe_cmd;
            last_pipe = pipe_cmd;
        }

        if (i < token_count && tokens[i].type == TOKEN_SEMICOLON) i++;

        if (!head) {
            head = current = cmd;
        } else {
            current->next = cmd;
            current = cmd;
        }
    }

    return head;
}
