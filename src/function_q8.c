
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


                // File for input and output redirection
                char *inputFile = NULL;
                char *outputFile = NULL;

                // Iterate through the arguments to check for input and output redirection
                for (size_t i = 0; i < arg_count; i++) {
                    // Input redirection
                    if (strcmp(args[i], "<") == 0) {
                        inputFile = args[i + 1];
                        args[i] = NULL; // Remove '<' from the argument list
                    }
                    
                    // Output redirection
                    else if (strcmp(args[i], ">") == 0) {
                        outputFile = args[i + 1];
                        args[i] = NULL; // Remove '>' from the argument list
                    }
                }

                // Handle input redirection
                if (inputFile != NULL) {
                    // Open the input file for reading
                    int fd = open(inputFile, O_RDONLY);
                    if (fd == -1) {
                        perror("Error: handleRedirection (Input)\nopen");
                        exit(EXIT_FAILURE);
                    }

                    // Redirect standard input to the file
                    if (dup2(fd, STDIN_FILENO) == -1) {
                        perror("Error: handleRedirection (Input)\ndup2");
                        close(fd);
                        exit(EXIT_FAILURE);
                    }
                    
                    // Close the file descriptor
                    close(fd);
                }

                // Handle output redirection
                if (outputFile != NULL) {
                    // Open the output file for writing
                    int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                    if (fd == -1) {
                        perror("Error: handleRedirection (Output)\nopen");
                        exit(EXIT_FAILURE);
                    }

                    // Redirect standard output to the file
                    if (dup2(fd, STDOUT_FILENO) == -1) {
                        perror("Error: handleRedirection (Output)\ndup2");
                        close(fd);
                        exit(EXIT_FAILURE);
                    }
                    
                    // Close the file descriptor
                    close(fd);
                }

                // Iterate through the arguments to check for pipe redirection
                for (size_t i = 0; i < arg_count; i++) {
                    if (strcmp(args[i], "|") == 0) {
                        // Set the pipe symbol to NULL to separate the first and second command
                        args[i] = NULL; 

                        // Split the arguments into two parts
                        char **firstCommand = &args[0];
                        char **secondCommand = &args[i + 1];

                        // Check for errors
                        int pipefd[2];
                        if (pipe(pipefd) == -1) {
                            perror("Error: handlePipe\npipe");
                            exit(EXIT_FAILURE);
                        }

                        pid_t childPid = fork();
                        if (childPid == -1) {
                            perror("Error: handlePipe\nfork");
                            exit(EXIT_FAILURE);
                        }

                        // Parent process: Execute the first command before the pipe
                        else if (childPid != 0) {
                            // Close the read end of the pipe since the parent writes to it
                            close(pipefd[0]);

                            // Redirect standard output to the write end of the pipe
                            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                                perror("Error: handlePipe (firstCommand)\ndup2");
                                close(pipefd[1]);
                                exit(EXIT_FAILURE);
                            }

                            // Close the write end of the pipe as it's no longer needed
                            close(pipefd[1]);

                            // Execute the first command using execvp
                            execvp(firstCommand[0], firstCommand);

                            // If execvp fails, print an error message
                            perror("Error: handlePipe (firstCommand)\nexecvp");
                            exit(EXIT_FAILURE);
                        }
                        
                        // Child process: Execute the second command after the pipe
                        else {
                            // Close the write end of the pipe since the child reads from it
                            close(pipefd[1]);

                            // Redirect standard input to the read end of the pipe
                            if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                                perror("Error: handlePipe (secondCommand)\ndup2");
                                close(pipefd[0]);
                                exit(EXIT_FAILURE);
                            }

                            // Close the read end of the pipe as it's no longer needed
                            close(pipefd[0]);

                            // Execute the second command using execvp
                            execvp(secondCommand[0], secondCommand);

                            // If execvp fails, print an error message
                            perror("Error: handlePipe (secondCommand)\nexecvp");
                            exit(EXIT_FAILURE);
                        }
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
