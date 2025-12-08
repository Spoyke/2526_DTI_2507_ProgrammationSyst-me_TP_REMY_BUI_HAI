#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define BUFFER_SIZE 128

int main(void) {
    char buffer[BUFFER_SIZE];

    strcpy(buffer, "Bienvenue dans le Shell ENSEA.\n"
                   "Pour quitter, tapez 'exit'\n");
    write(STDOUT_FILENO, buffer, strlen(buffer));

    ssize_t bytes;
    while (1) {
        strcpy(buffer, "enseah % ");
        write(STDOUT_FILENO, buffer, strlen(buffer));

        bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE);


        if (bytes < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}
