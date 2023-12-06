#include "function.h"

// Function to format and display a code with its value
void afficherCode(char *text, int val) {
    char buff[MAXSIZE];
    sprintf(buff, text, val); // Format the code using sprintf
    write(STDOUT_FILENO, buff, strlen(buff)); // Display the formatted code
}

// Function of the prompt
void welcome() {
    // Buffer size in function.h
    char commande[MAXSIZE];
    struct timespec timeStart, timeStop;
    int status;
    int ConvNsToMs = 1000000; // nanoseconds to milliseconds
    // Infinite loop for continuous interaction
    while (1) {
        // Display the enseah
        write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
        // Read user input
        ssize_t input_length = read(STDIN_FILENO, commande, MAXSIZE);
        // Check for read errors
        if (input_length == -1) {
            perror("Erreur de lecture");
            exit(EXIT_FAILURE);
        }
        // Check if the user wants to exit
        if (input_length == 0) {
            write(STDOUT_FILENO, "Bye Bye !\n", 12);
            exit(EXIT_SUCCESS);
        }

        // newline character from the user input
        commande[input_length - 1] = '\0';

        // Check if the user wants to exit
        if (strcmp(commande, "exit") == 0) {
            write(STDOUT_FILENO, "Bye Bye !\n", 12);
            exit(EXIT_SUCCESS);
        }
        // Record the start time
        clock_gettime(CLOCK_REALTIME, &timeStart);

        // Fork a new process
        pid_t ret = fork();

        // FORK's error
        if (ret == -1) {
            perror("Process Error"); // if fails print error msg
            exit(EXIT_FAILURE);
        }
        // Child process
        if (ret == 0) {
            // Check if the user entered a single character
            if (input_length == 1) {
                const char *date = "date";
                // We putt NULL to indicate that this is the end in the list of args. 
                char *date_args[] = {date, NULL};
                execvp(date, date_args); // Execute the date command
                perror("execvp");
                exit(EXIT_FAILURE);

            } else {
                char **new_buf = malloc(argc * sizeof(char*));
		        int file;
		
                for (int i = 0; i < argc; i++) {
                    // Gère la redirection des commandes
                    if (strcmp(buf[i], ">") == 0) {
                        file = open(buf[i + 1], O_WRONLY | O_TRUNC);
                        if (file == -1) {
                            perror("file");
                            exit(EXIT_FAILURE);
                        }
                        if (dup2(file, STDOUT_FILENO) == -1) {
                            perror("dup2");
                            exit(EXIT_FAILURE);
                        }
                        break;
                    } else if (strcmp(buf[i], "<") == 0) {
                        file = open(buf[i + 1], O_RDONLY);
                        if (file == -1) {
                            perror("file");
                            exit(EXIT_FAILURE);
                        }
                        if (dup2(file, STDIN_FILENO) == -1) {
                            perror("dup2");
                            exit(EXIT_FAILURE);
                        }
                        break;
                    }
                    /* les arg de la commandes avant les symboles < ou >
                    sont copiés dans un nouveau tableau pour être éxécutés */
                    new_buf[i] = malloc(strlen(buf[i]));
                    strcpy(new_buf[i], buf[i]);
                }
                close(file);
                execvp(new_buf[0], new_buf); 
                exit(EXIT_SUCCESS);
            }
        } else {
            // Wait for the child process to finish
            wait(&status);
            // Record the stop time
            clock_gettime(CLOCK_REALTIME, &timeStop);

            // Display the exit status or signal information
            // along with execution time
            if (WIFEXITED(status)) {
                afficherCode("[code exit: %d | ", WEXITSTATUS(status));
                afficherCode("%d ms] \n", (timeStop.tv_nsec - timeStart.tv_nsec) / ConvNsToMs);
            } else if (WIFSIGNALED(status)) {
                afficherCode("[signal exit: %d | ", WTERMSIG(status));
                afficherCode("%d ms] \n", (timeStop.tv_nsec - timeStart.tv_nsec) / ConvNsToMs);
            }
        }
    }
}
