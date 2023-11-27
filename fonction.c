#include "fonction.h"


void accueil() {
	
	write(STDOUT_FILENO, msg_bienvenue, strlen(msg_bienvenue));
	write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
}
