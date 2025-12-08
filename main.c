#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define BUFFER_SIZE 128

int main(void) {
    char buffer[BUFFER_SIZE];

    strcpy(buffer, "Bienvenue dans le Shell ENSEA.\n"
                   "Pour quitter, tapez 'exit'\n");
    // Writting in the standard output
    write(STDOUT_FILENO, buffer, strlen(buffer));

    while (1) {
        strcpy(buffer, "enseah % ");
        write(STDOUT_FILENO, buffer, strlen(buffer));

        // Replace every character of buffer with 0 -> clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        // Reading user input in the standard input
        ssize_t bytes;
        bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        if (bytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // When there is no '\n' in buffer, it means that the user typed <ctrl> + d
        if (strchr(buffer, '\n') == NULL) {
            strcpy(buffer, "Bye bye...\n");
            write(STDOUT_FILENO, buffer, strlen(buffer));
            return EXIT_SUCCESS;
        }

        // Replace the '\n' with '\0' so that execlp execute for exemple "ls" instead of "ls\n"
        buffer[strlen(buffer) - 1] = '\0';

        // If the user typed exit, stop the program
        if (strcmp(buffer, "exit") == 0) {
            strcpy(buffer, "Bye bye...\n");
            write(STDOUT_FILENO, buffer, strlen(buffer));
        }

        int pid, status;
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { // Child code
            execlp(buffer, buffer, (char *)NULL);
            return EXIT_FAILURE;
        }
        else { // Father code
            wait(&status);
        }
    }
}
