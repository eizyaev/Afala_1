#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

//**************************************************************************************
// function name: signal_handle
// Description: new SIGTSTP & SIGINT handler for smash - sending SIGTSTP & SIGINT to fg process
// Parameters: int signal - number of signal to set handler to.
// Returns: void.
//**************************************************************************************
void signal_handle(int signal);

//**************************************************************************************
// function name: child_handle
// Description: new SIGCHLD handler for smash - removing terminated child process from job list
// Parameters: int signal - number of signal to set handler to.
// Returns: void.
//**************************************************************************************
void child_handle(int signal);

#endif

