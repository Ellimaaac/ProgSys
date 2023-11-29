#include "function.h"

// Function to format and display a code with its value
void afficherCode(char *text, int val){	//en entrée le text du code à afficher et sa valeur
	char buff[MAXSIZE];
	sprintf(buff,text, val);// Format the code using sprintf
	write(STDOUT_FILENO, buff, strlen(buff));// Display the formatted code
}

// Function of the prompt
void welcome() {
	// Buffer size in function.h
	char commande[MAXSIZE];
	struct timespec timeStart, timeStop;
	int status;
	int ConvNsToMs = 1000000;// nanoseconds to milliseconds
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
        // Check if the user wants to exit
        if(input_length == 0){
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
		if(input_length==1)
		{
			const char *date = "date";
			execlp(date,date, NULL); // Execute the date command
			exit(EXIT_FAILURE);
		}
		else {
		// Execute the user-entered command
        execlp(commande, commande, NULL); 
        // Never go because excelp goes out the prog
        perror("execlp");
        exit(EXIT_FAILURE);
		} 
    
    } else {
		// Wait for the child process to finish
        wait(&status);
        // Record the stop time
		clock_gettime(CLOCK_REALTIME, &timeStop);
        
        // Display the exit status or signal information 
        //along with execution time
        if (WIFEXITED(status)){
				afficherCode("[code exit: %d | ",WEXITSTATUS(status));
				afficherCode("%d ms] \n", (timeStop.tv_nsec - timeStart.tv_nsec)/ConvNsToMs);
				}
			else if (WIFSIGNALED(status)){
				afficherCode("[signal exit: %d | ", WTERMSIG(status));
				afficherCode("%d ms] \n", (timeStop.tv_nsec - timeStart.tv_nsec)/ConvNsToMs);
		}
    }
        
    }
}



void execut(char commande[MAXSIZE]){
	execlp(commande,commande, NULL);
	exit(EXIT_FAILURE);
}
