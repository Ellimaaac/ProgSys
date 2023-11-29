#include "function.h"

// Function of the prompt
void welcome() {
	// Buffer size in function.h
	char commande[MAXSIZE];
	int status;
	
	// Infinite loop for continuous interaction
	while(1)
	{
		// Display the enseah
		write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
		
		// Read user input
		ssize_t input_length = read(STDIN_FILENO, commande, MAXSIZE);
        
        // Check for read errors
        if (input_length == -1) {
            perror("Erreur de lecture");
            exit(EXIT_FAILURE);
        } 
        
    //newline character from the user input
    commande[input_length - 1] = '\0';
    	
	// Fork a new process
    pid_t ret = fork();
	
	// FORK's error
    if (ret == -1) 
    {
        perror("Process Error"); // if fails print error msg
        exit(EXIT_FAILURE);
    }
    
    // Child process
    if (ret == 0) 
    {
		// Check if the entered command is "enter"
		if(input_length==1)
		{
			const char *date = "date";
			execlp(date,date, NULL); // exec date
			exit(EXIT_FAILURE);
		}
		else 
		{
		// Execute the user-entered command
        execlp(commande, commande, NULL); 
        // Never go because excelp goes out the prog
        perror("execlp"); 
        exit(EXIT_FAILURE);
		} 
    } 
    else 
		{
		// Wait for the child process to finish
        wait(&status);
       
		} 
    }
}




