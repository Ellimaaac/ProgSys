#include "fonction.h"

// Function to display a welcome message
void accueil() {
	// Display the welcome message
	write(STDOUT_FILENO, msg_bienvenue, strlen(msg_bienvenue));
	write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
}
