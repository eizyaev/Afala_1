// signals.c
// contains signal handler functions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "commands.h"

void signal_handle(int signal) 
{
    list<job>::iterator it;
    if (fg_job != NULL && fg_job->is_fg == true)	// Checking if the last job we opened still exists & if it runs in fg //
    {
        switch(signal)
        {
            case (SIGTSTP):	// CTRL-Z //
                fprintf(stdout, "smash > signal SIGTSTP was sent to %d\n", fg_job->pid);
	            kill(fg_job->pid,SIGTSTP);	// Send SIGTSTP to the fg job //
                fg_job->is_running = false;
                fg_job->is_fg = false;

				/* Searching for the job within job list, adding it to list if it isn't there */
                for( it = jobs->begin() ; it != jobs->end() ; it++)
                    if ((*it).pid == fg_job->pid)
                        break;
                if (it == jobs->end())
                {
                    new_job.id = job_cnt++;
                    new_job.timer = time(NULL);
                    jobs->push_back(new_job);
                    fg_job = &(jobs->back());
                }

                break;
            case (SIGINT):	// CTRL-C //
                    fg_job->is_fg = false;
                    fprintf(stdout, "smash > signal SIGINT was sent to %d\n", fg_job->pid);	
	                kill(fg_job->pid,SIGINT);
                break;
            default:
                break;
        }
    }
}

void child_handle(int signal)
{
    pid_t pid = waitpid(-1, NULL, WNOHANG);	// Checking if any child process has terminated //
    if (pid > 0)
    {
		/* Checking if we're handling fg job */
        if (fg_job != NULL && fg_job->pid == pid)
            fg_job = NULL;
    }
    while(pid > 0)
    {       
		/* Erasing the child process from the job list */
        list<job>::iterator it;
        for( it = jobs->begin() ; it != jobs->end() ; it++)
        {
            if ((*it).pid == pid)
            {
                jobs->erase(it);
                break;
            }
        }
        pid = waitpid(-1, NULL, WNOHANG);	// Checking if any child process has terminated //
    }
}

