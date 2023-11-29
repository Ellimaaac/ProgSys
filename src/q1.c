#include "fonction.h"
#include "fonction.c"

int main(){
	
	//Call function
	write(STDOUT_FILENO, msg_welcome, strlen(msg_welcome));
	welcome();
	
	return 1;
	}
