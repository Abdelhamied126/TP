#include <stdio.h>
#include <unistd.h>

#define message "$ ./enseash\n Bienvenue dans le Shell ENSEA.\n Pour quitter, tapez 'exit'.\n enseash %"

int main() {
    write(STDOUT_FILENO, message, strlen(message));
    return 0;
}
