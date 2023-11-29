#include "fonction.h"

// Function to display a welcome message
void welcome() {
	// Display the welcome message
	write(STDOUT_FILENO, msg_welcome, strlen(msg_welcome));
	write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
}
