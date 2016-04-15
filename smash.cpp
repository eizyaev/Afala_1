/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

char* L_Fg_Cmd;
int job_cnt;
list<job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
/*
 * argc stands for "argument count" - argc contains the number of arguments passed to the program
 * argv stands for "argument vector". A vector is a one-dimensional array,
 * and argv is a one-dimensional array of strings
 */
{
    char cmdString[MAX_LINE_SIZE]; 	   

    struct sigaction sa;
    sa.sa_handler = &handle_bg;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);

    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        perror("Error: cannot handle SIGTSTP"); 
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 


	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) // malloc failed
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// perform a complicated Command
		//if(!ExeComp(lineSize)) continue; 
					// background command	
	 	//if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

