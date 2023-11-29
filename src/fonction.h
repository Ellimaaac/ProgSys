// Libraries
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
#define msg_welcome "Bienvenue dans ENSEA Shell.\nPour quitter, tapez 'exit'. \n"
#define msg_enseash "enseash%"
#define MAXSIZE 1024

//fonction of welcome message
void welcome(void);
void execut(char commande[MAXSIZE]);
