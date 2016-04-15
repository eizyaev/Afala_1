// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "commands.h"

void handle_bg(int signal) 
{
    pid_t pID = jobs.front().pid;
    cout << "stoped!!!" << endl;
	kill(pID,SIGSTOP);
}
