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
    if (fg_job != NULL && fg_job->is_fg == true)
    {
        switch(signal)
        {
            case (SIGTSTP): // CTR-Z
                fprintf(stdout, "smash > signal SIGTSTP was sent to %d\n", fg_job->pid); // TODO fix message
	            kill(fg_job->pid,SIGTSTP);
                fg_job->is_running = false;
                fg_job->is_fg = false;
                for( it = jobs->begin() ; it != jobs->end() ; it++)
                    if ((*it).pid == fg_job->pid)
                        break;
                if (it == jobs->end())
                {
                    new_job.id = job_cnt++; 
                    jobs->push_back(new_job);
                    fg_job = &(jobs->back());
                }

                break;
            case (SIGINT): // CTR-C
                    fg_job->is_fg = false;
                    fprintf(stdout, "smash > signal SIGINT was sent to %d\n", fg_job->pid); // TODO fix message
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
    if (pid > 0)
    {
        if (fg_job != NULL && fg_job->pid == pid)
            fg_job = NULL;
        /*
        if (new_job.pid == pid)
        {
            new_job.id = 0;
            new_job.cmd = "";
            new_job.pid = 0;
            new_job.is_running = 0;
        }
        */
    }
    while(pid > 0)
    {       
        list<job>::iterator it;
        for( it = jobs->begin() ; it != jobs->end() ; it++)
        {
            if ((*it).pid == pid)
            {
                jobs->erase(it);
                break;
            }
        }
        pid = waitpid(-1, NULL, WNOHANG);
    }
}

