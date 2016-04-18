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
    if (fg_job != NULL)
    {
        switch(signal)
        {
            case (SIGTSTP):
                cout << "SIGTSTP" << endl;
	            kill(fg_job->pid,SIGTSTP);
                fg_job->is_fg = false;
                for( it = jobs->begin() ; it != jobs->end() ; it++)
                    if ((*it).pid == fg_job->pid)
                        break;
                if (it == jobs->end())
                {
                    new_job.id = job_cnt++; 
                    jobs->push_back(new_job);
                    fg_job = &(jobs->back());
                    new_job.id = 0;
                    cout << "Inserted to the jobs list" << endl;
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
    if (fg_job->pid == pid)
        fg_job = NULL;
    if (new_job.pid == pid)
    {
        cout << "job " << pid << "erased" << endl;
        new_job.id = 0;
        new_job.cmd = "";
        new_job.pid = 0;
        new_job.is_fg = 0;
    }
    while(pid > 0)
    {       
        list<job>::iterator it;
        cout << "fg job pid: " << fg_job->pid << endl;
        for( it = jobs->begin() ; it != jobs->end() ; it++)
            if ((*it).pid == pid)
            {
                jobs->erase(it);
                break;
            }
        pid = waitpid(-1, NULL, WNOHANG);
        cout << "SIGCHLD PID IS:" << pid << "Is next" << endl;
    }
}

