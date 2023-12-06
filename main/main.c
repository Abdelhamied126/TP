#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>
#include <stdlib.h>

#define byebye_ctrl_plus_d "Bye bye Ctrl+D...\n$"

int main() {
    char Buf[256];
    char prompt[] = "enseash %% ";
    char exitPrompt[] = "enseash [exit:";
    char signPrompt[] = "enseash [sign:";
    write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);

    while (1)
    {
        int status;

        ssize_t cmd_size = read(STDIN_FILENO, Buf, sizeof(Buf) - 1);//Reading the Entered Command
        Buf[cmd_size - 1] = '\0';  //Indicate the end of the message
        //////////////////////////////////////////////////////////////////
        //Handling the shell output with the "exit" command or a <ctrl>+d;//
        if (strcmp(Buf, "exit") == 0)
        {
            break;
        }
        if (cmd_size==0)//"<ctrl>+d" command
        {
            write(STDOUT_FILENO, byebye_ctrl_plus_d, strlen(byebye_ctrl_plus_d));
            break;
        }

        pid_t pid = fork();
        if (pid == -1)//Handle error
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            //Treat the child
            execlp(Buf, Buf, (char *)NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else {
            // Code du processus parent

            waitpid(pid, &status, 0);//Wait for the thread to finish(child)
            //Display of return code (or signal)//
            if (WIFEXITED(status))//True if the child ended normally (exit or return)
            {
                write(STDOUT_FILENO, exitPrompt, sizeof(exitPrompt) - 1);
                dprintf(STDOUT_FILENO, "%d", WEXITSTATUS(status));//Returns the output value of the child
                write(STDOUT_FILENO, "] %% ", 5);
            } else if (WIFSIGNALED(status))//true if the child ended because of a signal
            {
                write(STDOUT_FILENO, signPrompt, sizeof(signPrompt) - 1);
                dprintf(STDOUT_FILENO, "%d", WTERMSIG(status));//returns the number of the signal that caused the termination
                write(STDOUT_FILENO, "] %% ", 5);
            }
        }
    }

    return 0;
}