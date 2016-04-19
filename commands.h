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

typedef struct job // TODO: add time
{
    int id;
    string cmd;
    pid_t pid;
    bool is_running;
    bool is_fg;
}job;


extern list<job> *jobs;
extern int job_cnt;
extern job* fg_job;
extern job new_job;
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
int ExeComp(char* lineSize, bool bg);
int BgCmd(char* lineSize);
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
#endif

