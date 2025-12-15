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
#define MAX_ARGS 16

ssize_t searchChar(const char * str, size_t size, char c);
size_t strslice(char ** dst, const char * src);

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
            char * args[MAX_ARGS];
            strslice(args, buffer);



            execvp(args[0], args);
            return EXIT_FAILURE;
        }

        // Father code
        wait(&status);

        strncpy(buffer, "", BUFFER_SIZE);

        // Declare a array of 4 char to match the size of an int to convert status (int) into a string (char *)
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

size_t strslice(char ** dst, const char * src) {
    /* Create an array of small strings of src every time there is a ' ' separating to character
     * Example : src = "  ls -a -b c\0" return (in dst) ["ls\0", "-a\0", "-b\0", "c\0", NULL]
     * Return the size of dst
    */
    int arg_count = 0;
    int i = 0;

    while (src[i] != '\0') {
        // Go through the string until there is a character other than ' ' -> start slicing at the start of a word
        while (src[i] == ' ' && src[i + 1] != '\0') {
            i++;
        }

        if (src[i] == '\0') {
            break;
        }

        // Go through the word and storeit lenght
        int len = 0;
        while (src[i + len] != ' ' && src[i + len] != '\n' && src[i + len] != '\0') {
            len++;
        }

        dst[arg_count] = malloc(sizeof(char) * (len + 1));
        // Error during malloc
        if (dst[arg_count] == NULL) {
            return -1;
        }
        // Copy the word into the memory allocated with malloc
        strncpy(dst[arg_count], &src[i], len);
        arg_count++;
        i += len;
    }

    dst[arg_count] = NULL;
    return arg_count;
}

