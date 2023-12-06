#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>
#include <stdlib.h>



#define MESSAGE "$ ./enseash\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash %"
#define BUFFER_SIZE 128
#define enseash "enseash%"
#define byebye "Bye bye...\n$"

int main () {
    char Buf[BUFFER_SIZE];
    int pid, status;
    ssize_t cmd_size;

    write(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));
    while(1)
    {
        cmd_size = read(STDIN_FILENO, Buf, BUFFER_SIZE - 1);//Reading the Entered Command
        write(STDOUT_FILENO, Buf, strlen(Buf) - 1);//Printing the Entered Command
        Buf[cmd_size - 1] = 0; //Indicate the end of the message
        pid = fork();

        printf("Bytes read: %zd\n", cmd_size);

        if (pid != 0)//Wait for the thread to finish(child)
        {
            wait(&status);
        } else {
            execlp(Buf, Buf, (char *) NULL);//Treat the child
        }
        write(STDOUT_FILENO, enseash, strlen(enseash));
        //Handling the shell output with the "exit" command or a <ctrl>+d;//
        if (strcmp(Buf,"exit")==0)//"exit" command
        {
            write(STDOUT_FILENO, byebye, strlen(byebye));
            exit(-1);
        }
        if (cmd_size==0)//"<ctrl>+d" command
        {
            write(STDOUT_FILENO, byebye, strlen(byebye));
            exit(-1);
        }
    }
    return 0;
}
