#include "function.h"

// Function of the prompt
void welcome() {
	// Define a buffer
	char commande[MAXSIZE];
	int status;
	// Infinite loop for continuous interaction
	while(1)
	{
		// Display the welcome message
		write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
		
		// Read user input
		ssize_t input_length = read(STDIN_FILENO, commande, MAXSIZE);
        
        // Check for read errors
        if (input_length == -1) {
            perror("Erreur de lecture");
            exit(EXIT_FAILURE);
        } 
        
		else 
		{
		// Wait until exit
        wait(&status);
       
		} 
    }
}




