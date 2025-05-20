#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

void execute_pipeline(Command *cmd) {
    int fd[2], in_fd = 0;

    while (cmd) {
        pipe(fd);
        pid_t pid = fork();

        if (pid == 0) {
            dup2(in_fd, 0); // read from previous
            if (cmd->pipe_to) dup2(fd[1], 1); // write to pipe

            close(fd[0]);
            execvp(cmd->args[0], cmd->args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
            return;
        }

        wait(NULL);
        close(fd[1]);
        in_fd = fd[0];
        cmd = cmd->pipe_to;
    }
}

void execute_command_chain(Command *cmd) {
    while (cmd) {
        if (cmd->args[0] == NULL) {
            cmd = cmd->next;
            continue;
        }

        if (strcmp(cmd->args[0], "cd") == 0) {
            if (cmd->args[1] == NULL || chdir(cmd->args[1]) != 0)
                perror("cd");
        } else if (strcmp(cmd->args[0], "exit") == 0) {
            exit(0);
        } else if (strcmp(cmd->args[0], "history") == 0) {
            print_history();
        } else {
            execute_pipeline(cmd);
        }

        cmd = cmd->next;
    }
}

void free_command_chain(Command *cmd) {
    while (cmd) {
        for (int i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);

        Command *next = cmd->next;
        free(cmd);
        cmd = next;
    }
}
