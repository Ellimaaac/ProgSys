
#include "function.h"
#include <time.h>

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
                // Tokenize the command and arguments
                char *token = strtok(commande, " ");
                char *args[MAXSIZE];
                int arg_count = 0;

                while (token != NULL) {
                    args[arg_count++] = token;
                    token = strtok(NULL, " ");
                }
                args[arg_count] = NULL;

		        int file, input_redirection, output_redirection;
		
               for (int i = 0; i < arg_count; i++) {

                    // Handle input redirection
                    if (strcmp(args[i], "<") == 0) {
                        if (i + 1 < arg_count) {
                            file = open(args[i + 1], O_RDONLY);

                            if (file == -1) {
                                perror("open");
                                exit(EXIT_FAILURE);
                            }

                            if (dup2(file, STDIN_FILENO) == -1) {
                                perror("dup2");
                                exit(EXIT_FAILURE);
                            }

                            close(file); // Close the file descriptor after dup2
                            input_redirection = 1;
                            
                        } else {
                            fprintf(stderr, "Missing input file after '<'.\n");
                            exit(EXIT_FAILURE);
                        }

                    }
                    // Handle output redirection
                    else if (strcmp(args[i], ">") == 0) {
                        if (i + 1 < arg_count) {
                            file = open(args[i + 1], O_CREAT | O_TRUNC | O_WRONLY);

                            if (file == -1) {
                                perror("open");
                                exit(EXIT_FAILURE);
                            }
                            if (dup2(file, STDOUT_FILENO) == -1) {
                                perror("dup2");
                                exit(EXIT_FAILURE);
                            }

                            close(file); // Close the file descriptor after dup2
                            output_redirection = 1;

                        } else {
                            fprintf(stderr, "Missing output file after '>'.\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                }

                // If there's no explicit output redirection, restore stdout
                if (!output_redirection) {
                    if (dup2(STDOUT_FILENO, STDOUT_FILENO) == -1) {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                }

                // If there's no explicit input redirection, restore stdin
                if (!input_redirection) {
                    if (dup2(STDIN_FILENO, STDIN_FILENO) == -1) {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                }

                execvp(args[0], args);
                // If execvp fails, print an error message
                perror("execvp");
                exit(EXIT_FAILURE); 
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
