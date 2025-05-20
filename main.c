#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

char *history[MAX_HISTORY];
int history_count = 0;

void add_to_history(const char *cmd) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(cmd);
    } else {
        free(history[0]);
        for (int i = 1; i < MAX_HISTORY; i++)
            history[i - 1] = history[i];
        history[MAX_HISTORY - 1] = strdup(cmd);
    }
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}

int main() {
    char input[MAX_CMD_LENGTH];

    while (1) {
        printf("myshell> ");
        if (fgets(input, MAX_CMD_LENGTH, stdin) == NULL) break;

        input[strcspn(input, "\n")] = '\0';
        add_to_history(input);

        int token_count = 0;
        Token *tokens = tokenize_input(input, &token_count);
        Command *cmd_chain = parse_tokens(tokens, token_count);
        execute_command_chain(cmd_chain);
        free_command_chain(cmd_chain);

        for (int i = 0; i < token_count; i++)
            free(tokens[i].value);
        free(tokens);
    }

    for (int i = 0; i < history_count; i++)
        free(history[i]);

    return 0;
}
