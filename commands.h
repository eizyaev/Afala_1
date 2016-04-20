#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Extra includes */
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <set> // TODO: can we use this?
using namespace std;

/* Structure for jobs */
typedef struct job
{
    int id;
    string cmd;
    pid_t pid;
    bool is_running;
    bool is_fg;
    time_t timer;
}job;


extern list<job> *jobs;	// Job list //
extern int job_cnt;	// Counter to keep track of job ammount & numbering the jobs in job list //
extern job* fg_job;	// Pointer to the most current job we ran //
extern job new_job;	// Helper job struct for handling jobs before storing them in job list //
#define MAX_LINE_SIZE 80
#define MAX_ARG 20

int ExeComp(char* lineSize, bool bg);	// Complicated CMD execution, can also be complicated bg CMD //
int BgCmd(char* lineSize);	// Execution of bg CMDs - also calling 'ExeComp' function for complicated bg CMDs //
int ExeCmd(char* lineSize, char* cmdString);	// Execution of built-in CMDs //
void ExeExternal(char *args[MAX_ARG], char* cmdString);	// Execution of external CMDs //
#endif

