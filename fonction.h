// include

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// define
#define msg_bienvenue "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'. \n"
#define msg_enseash "enseash% "
#define msg_exit "Bye bye ...\n"
#define MAXSIZE 1024

//fonction
void accueil(void);
