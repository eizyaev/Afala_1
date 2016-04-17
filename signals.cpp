// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "commands.h"

void signal_handle(int signal) 
{
    list<job>::iterator it;
    int status;
    if (fg_job != NULL)
    {
        switch(signal)
        {
            case (SIGTSTP):
                cout << "SIGTSTP" << endl;
	            kill(fg_job->pid,SIGTSTP);
                for( it = jobs->begin() ; it != jobs->end() ; it++)
                    if ((*it).pid == fg_job->pid)
                        break;
                if (it != jobs->end())
                {
                    new_job.id = job_cnt++; 
                    jobs->push_back(new_job);
                    fg_job = &(jobs->back());
                }

                break;
            case (SIGINT):
                cout << "SIGINT" << endl;
	            kill(fg_job->pid,SIGINT);
                break;
            default:
                break;
        }
    }
}

void child_handle(int signal)
{
    pid_t pid = waitpid(-1, NULL, WNOHANG);
    cout << "SIGCHLD PID IS:" << pid << endl;
    while(pid > 0)
    {        
        list<job>::iterator it;
    
        for( it = jobs->begin() ; it != jobs->end() ; it++)
            if ((*it).pid == pid)
            {
                if (fg_job == &(*it))
                    fg_job == NULL;
                jobs->erase(it);
                break;
            }
        pid = waitpid(-1, NULL, WNOHANG);
        cout << "SIGCHLD PID IS:" << pid << endl;
    }
}

