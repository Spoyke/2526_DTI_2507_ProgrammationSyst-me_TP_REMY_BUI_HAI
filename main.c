#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <time.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define CV 1000000

#define BUFFER_SIZE 128

int main(void) {
    char buffer[BUFFER_SIZE];


    struct timespec start, end;

    strcpy(buffer, "Bienvenue dans le Shell ENSEA.\n"
                   "Pour quitter, tapez 'exit'\n");
    // Writting in the standard output
    write(STDOUT_FILENO, buffer, strlen(buffer));

    strcpy(buffer, "enseah % ");
    write(STDOUT_FILENO, buffer, strlen(buffer));

    while (1) {
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
            return EXIT_SUCCESS;
        }

        int pid, status;
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { // Child code
            if (clock_gettime(CLOCK_REALTIME, &start)==-1){
                perror("clock_gettime");
                exit(EXIT_FAILURE);
            }
            execlp(buffer, buffer, (char *)NULL);
            return EXIT_FAILURE;
        }

        else { // Father code
            wait(&status);

            if (clock_gettime(CLOCK_REALTIME, &end)==-1){
                perror("clock_gettime");
                exit(EXIT_FAILURE);
            }


            memset(buffer, 0, BUFFER_SIZE);
            strcat(buffer, "enseah [");

            char status_str[4];
            char elapsed_time[15] = {0};
            long nanosec = end.tv_nsec - start.tv_nsec;
            nanosec = nanosec /  CV;

            // If the return value of the child is an exit value
            if (WIFEXITED(status)) {
                strcat(buffer, "exit:");
                // Convert the return value (int) to a string (char *)
                sprintf(status_str, "%d", WEXITSTATUS(status));
                sprintf(elapsed_time, "%ld", nanosec);
                snprintf(buffer, BUFFER_SIZE, "enseah [exit:%s|%sms] %% ", status_str, elapsed_time);
            }
            // If the return value of the child is a signal
            else if (WIFSIGNALED(status)) {
                strcat(buffer, "sign:");
                sprintf(status_str, "%d", WTERMSIG(status));
                sprintf(elapsed_time, "%ld", nanosec);
                snprintf(buffer, BUFFER_SIZE, "enseah [exit:%s|%sms] %% ", status_str, elapsed_time);
            }

            strcat(buffer, status_str);
            strcat(buffer, "] % ");
            write(STDOUT_FILENO, buffer, strlen(buffer));
        }
    }
}