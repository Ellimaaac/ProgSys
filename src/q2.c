#include "fonction.h"



int main(void) {
    int status;
    int nbChar;
    char commande[MAXSIZE];

	// Read input
    nbChar = read(STDIN_FILENO, commande, MAXSIZE);

    // If User just pressed Enter without entering command
    if (nbChar == 1) {
        const char *date = "date";
        execlp(date, date, NULL); // display date
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    
     // Otherwise, continue with the original logic of forking and executing the entered command
    commande[nbChar - 1] = '\0';

	// Fork a new process
    pid_t ret = fork();
	
	// FORK's error
    if (ret == -1) {
        perror("Process Error"); // if fails print error msg
        exit(EXIT_FAILURE);
    }
    // Child process
    if (ret == 0) {
        execlp(commande, commande, NULL); //exe child 
        // n'arrive jamais car excelp sort duu prog
        perror("execlp"); // if fails print error msg
        exit(EXIT_FAILURE);
    } 
    
    else {
		// Wait for the child process to finish
        wait(&status);
        input_length--;
    }

    return 0;
}
