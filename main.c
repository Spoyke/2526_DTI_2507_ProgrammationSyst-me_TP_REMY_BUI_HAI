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
#define INT_SIZE 4
#define CHAR_SIZE 1
#define MAX_ARGS 16

ssize_t searchChar(const char * str, size_t size, char c);
int strslice(char ** dst, char * src);
int changestdout(char * path);
int changestdin(char * path);

int main(void) {
    char buffer[BUFFER_SIZE];

    // Writing in the standard output
    strncpy(buffer,"Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'\n", BUFFER_SIZE);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    strncpy(buffer, "enseah % ", BUFFER_SIZE);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    while (1) {
        // Reading user input in the standard input
        const ssize_t bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        if (bytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // When there is no '\n' in buffer, it means that the user typed <ctrl> + d
        if (searchChar(buffer, bytes, '\n') == -1) {
            strncpy(buffer, "Bye bye...\n", BUFFER_SIZE);
            write(STDOUT_FILENO, buffer, BUFFER_SIZE);
            return EXIT_SUCCESS;
        }

        // Replace the '\n' with '\0' so that execlp execute for exemple "ls" instead of "ls\n"
        buffer[bytes - 1] = '\0';

        // If the user typed exit, stop the program
        if (strncmp(buffer, "exit", bytes) == 0) {
            strncpy(buffer, "Bye bye...\n", BUFFER_SIZE);
            write(STDOUT_FILENO, buffer, BUFFER_SIZE);
            return EXIT_SUCCESS;
        }

        int status;
        const int pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { // Child code
            // Declare a variable to store the arguments of buffer.
            // Size BUFFER_SIZE so that we can store every character of buffer into args
            char * args[BUFFER_SIZE];
            int nb_args = strslice(args, buffer);

            for (int i = 0; i < nb_args; i++) {
                if (strncmp(args[i], ">", CHAR_SIZE) == 0 && i + 1 < nb_args) {
                    if (changestdout(args[i + 1]) == -1) {
                        perror("changestdout");
                        return EXIT_FAILURE;
                    }

                    args[i] = NULL;
                    i++;
                    continue;
                }

                if (strncmp(args[i], "<", CHAR_SIZE) == 0 && i + 1 < nb_args) {
                    if (changestdin(args[i + 1]) == -1) {
                        perror("changestdin");
                        return EXIT_FAILURE;
                    }

                    args[i] = NULL;
                    i++;
                }
            }

            execvp(args[0], args);
            return EXIT_FAILURE;
        }

        // Father code
        wait(&status);

        strncpy(buffer, "", BUFFER_SIZE);

        // Declare an array of 4 char to match the size of an int to convert status (int) into a string (char *)
        char status_str[INT_SIZE];

        // If the child stopped properly
        if (WIFEXITED(status)) {
            // Convert the return value (int) to a string (char *)
            sprintf(status_str, "%d", WEXITSTATUS(status));
            snprintf(buffer, BUFFER_SIZE, "enseah [exit:%s] %% ", status_str);
        }
        // If the child was stopped by a signal
        else if (WIFSIGNALED(status)) {
            sprintf(status_str, "%d", WTERMSIG(status));
            snprintf(buffer, BUFFER_SIZE, "enseah [sign:%s] %% ", status_str);
        }

        write(STDOUT_FILENO, buffer, BUFFER_SIZE);
    }
}

ssize_t searchChar(const char * str, const size_t size, char c) {
    // Return the index of c in string if c is in string else return -1
    for (size_t i = 0; str[i] != '\0' && i < size; i++) {
        if (str[i] == c) {
            return i;
        }
    }
    return -1;
}

int strslice(char ** dst, char * src) {
    /* Create an array of small strings of src every time there is a ' ' separating to character
     * Example : src = "ls -a -b c\0" return (in dst) ["ls\0", "-a\0", "-b\0", "c\0", NULL]
     * Return the size of dst
    */

    int i = 0;

    // Store a pointer of the string that start from the beginning of src until the character ' '
    char *token = strtok(src, " ");

    while (token != NULL) {
        // Store the string into dst
        dst[i] = token;
        i++;

        token = strtok(NULL, " ");
    }

    dst[i] = NULL;

    return i;
}

int changestdout(char * path) {
    int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    int new_fd = dup2(fd, STDOUT_FILENO);

    if (new_fd == -1) {
        perror("dup2");
        return -1;
    }

    return 0;
}

int changestdin(char * path) {
    int fd = open(path, O_RDONLY);
    int new_fd = dup2(fd, STDIN_FILENO);

    if (new_fd == -1) {
        perror("dup2");
        return -1;
    }

    return 0;
}