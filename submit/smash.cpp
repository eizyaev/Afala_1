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

job new_job;	// Helper job struct for handling jobs before storing them in job list //
job* fg_job = NULL;	// Pointer to the most current job we ran //
int job_cnt = 1;	// Counter to keep track of job ammount & numbering the jobs in job list //
list<job> *jobs;	// Job list //
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
    jobs = new list<job>;

	/* Adjusting new handlers for SIGTSTP, SIGINT & SIGCHLD */
    struct sigaction sa;
    sa.sa_handler = &signal_handle;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);

    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        perror("Error: cannot handle SIGTSTP"); 

    if (sigaction(SIGINT, &sa, NULL) == -1)
        perror("Error: cannot handle SIGINT");

    signal (SIGCHLD, &child_handle);
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
		
	 	if(!BgCmd(lineSize)) continue;	// Checking if CMD is bg CMD, executing if it is (can be a complicated bg CMD) //
        if(!ExeComp(lineSize, false)) continue;		// Checking if CMD is complicated+not bg CMD, executing if it is //
		ExeCmd(lineSize, cmdString);	// Executing built-in/external CMD //
		
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}


