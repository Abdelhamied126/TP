#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_INPUT_SIZE 1024

int last_exit_status = 0;
int last_signal = 0;
struct timespec start_time, end_time;

void welcome_message() {
    const char welcome[] = "Welcome to the Mini Shell ENSEA.\nEnter 'exit' to quit.\n";
    write(STDOUT_FILENO, welcome, strlen(welcome));
}

void display_prompt() {

    char prompt[MAX_INPUT_SIZE];
    if (last_exit_status != 0) {
        // Build the prompt with exit indication and execution time
        snprintf(prompt, sizeof(prompt), "enseash [exit:%d|%ldms] %% ", last_exit_status,
                 (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_nsec - start_time.tv_nsec) / 1000000);
    }
        // Check if the last signal is non-zero
    else if (last_signal != 0) {
        // Build the prompt with signal indication and execution time
        snprintf(prompt, sizeof(prompt), "enseash [signal:%d|%ldms] %% ", last_signal,
                 (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_nsec - start_time.tv_nsec) / 1000000);
    }
    else {
        snprintf(prompt, sizeof(prompt), "enseash %% ");
    }

    // Display the prompt
    write(STDOUT_FILENO, prompt, strlen(prompt));
}
void execute_command(char *command) {
    char *args[MAX_INPUT_SIZE];
    char *token;

    // Parse the command and arguments
    int argc = 0;
    token = strtok(command, " ");
    while (token != NULL) {
        args[argc++] = token;
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    //creating process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Child process
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        int status;
        waitpid(pid, &status, 0);

        clock_gettime(CLOCK_MONOTONIC, &end_time);
        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
            last_signal = 0;
        } else if (WIFSIGNALED(status)) {
            last_exit_status = 0;
            last_signal = WTERMSIG(status);
        } else {
            last_exit_status = 0;
            last_signal = 0;
        }
    }
}
int main() {
    char input[MAX_INPUT_SIZE];

    welcome_message();

    while (1) {
        display_prompt();

        // Read user input
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input));

        if (bytes_read == 0) {
            // Handle end of file (Ctrl+D)
            const char goodbye[] = "\nGoodbye...\n";
            write(STDOUT_FILENO, goodbye, strlen(goodbye));
            break;
        }

        // Remove the newline character
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            const char goodbye[] = "Goodbye...\n";
            write(STDOUT_FILENO, goodbye, strlen(goodbye));
            break;
        }

        // Execute the entered command
        execute_command(input);
    }

    return 0;
}

