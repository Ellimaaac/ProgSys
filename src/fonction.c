#include "fonction.h"

void afficherCode(char *text, int val){	//en entrée le text du code à afficher et sa valeur
	char buff[MAXSIZE];
	sprintf(buff,text, val);
	write(STDOUT_FILENO, buff, strlen(buff));
}

// Function to display a welcome message
void welcome() {
	char commande[MAXSIZE];
	struct timespec timeStart, timeStop;
	int status;
	int ConvNsToMs = 1000000;
	while(1)
	{
		// Display the welcome message
		
		write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
		
		ssize_t input_length = read(STDIN_FILENO, commande, MAXSIZE);
        
        if (input_length == -1) {
            perror("Erreur de lecture");
            exit(EXIT_FAILURE);
        } 
        
        if(input_length == 0){
			write(STDOUT_FILENO, "Bye Bye !\n", 12);
			exit(EXIT_SUCCESS);	
		}
        
  
     // Otherwise, continue with the original logic of forking and executing the entered command
    commande[input_length - 1] = '\0';
    
	// Check if the user wants to exit
	if (strcmp(commande, "exit") == 0) {
		write(STDOUT_FILENO, "Bye Bye !\n", 12);
		exit(EXIT_SUCCESS);
	} 

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
		if(input_length==1)
		{
			const char *date = "date";
			execlp(date,date, NULL); // display date
			exit(EXIT_FAILURE);
		}
		else {
        execlp(commande, commande, NULL); //exe child 
        // n'arrive jamais car excelp sort duu prog
        perror("execlp"); // if fails print error msg
        exit(EXIT_FAILURE);
		} 
    
    } else {
		// Wait for the child process to finish
        wait(&status);
        
		clock_gettime(CLOCK_REALTIME, &timeStop);
        
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
