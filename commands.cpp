//		commands.c
//********************************************
#include "commands.h"
#include <sys/stat.h> // TODO: can we use this?
using namespace std;
char prev_pwd[MAX_LINE_SIZE] = "";

typedef struct vars
{
    string key;
    string data;

    vars(const string& strKey = "", const std::string& strData = "")
      : key(strKey),
        data(strData) {}

    bool operator<(const vars& rhs) const
    {
        return key < rhs.key;
    }

    bool operator==(const vars& rhs) const
    {
        return key == rhs.key;
    }
}Vars;

list<Vars> shell_vars;

list<job>::iterator find_job(int num) // TODO: add declaration
{
    list<job>::iterator it;
    
    for( it = jobs->begin() ; it != jobs->end() ; it++)
        if ((*it).id == num)
                break;
    return it;

}



//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString)
{
    string tmp;
    vector<string> s_args(MAX_ARG);
    list<Vars>::iterator it;
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char const* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
	}
    for (i = 1 ; i <= num_arg ; i++) // Variable to value parser
    {
        if (args[i][0] == '$')
        {
            string str(args[i]+1);
            it = find(shell_vars.begin(), shell_vars.end(), str);
            if (it != shell_vars.end())
                s_args[i] = (*it).data;
        }
        else
            s_args[i] = args[i];
    }


/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
        if (num_arg == 0)
        {
            fprintf(stderr, "smash error: > \"\" not found\n");
            return 1;
        }

        getcwd(pwd, sizeof(pwd));
        if (s_args[1] == "-")
            if (strcmp(prev_pwd, ""))
            {
                chdir(prev_pwd);
                strcpy(prev_pwd, pwd);
                return 0;
            }
            else
            {
                fprintf(stderr, "smash error: > none previouse directory\n");
                return 1;
            }
        else if (chdir(s_args[1].c_str()) == 0) 
        {
            strcpy(prev_pwd, pwd);
            return 0;
        }
        else
        {
            fprintf(stderr, "smash error: > \"%s\" not found\n", s_args[1].c_str());
            return 1; 
        }
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
        if (getcwd(pwd, sizeof(pwd)) != NULL)
        {
            fprintf(stdout, "%s\n", pwd);
            return 0;
        }
        else
        {
            perror("getcwd() error");
            return 1;
        }
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
		if (num_arg == 0)
		{
			fprintf(stderr, "smash error: > \"%s\" - cannot create directory\n", cmd);
			return 1;
		}
		else
		{
			getcwd(pwd, sizeof(pwd));
			char pwd_helper[MAX_LINE_SIZE];
			strcpy(pwd_helper, pwd);
			strcat(pwd_helper, "/");
			strcat(pwd_helper, s_args[1].c_str());

			if (chdir(pwd_helper) == 0)
			{
				chdir(pwd);
				fprintf(stderr, "smash error: > \"%s\" - directory already exists\n", cmd);
                return 1;
			}

			if (mkdir(pwd_helper, S_IRWXU | S_IRGRP | S_IXGRP | S_IXOTH | S_IROTH ) != 0)
			{
				fprintf(stderr, "smash error: > \"%s\" - cannot create directory\n", cmd);
				return 1;
			}

			return 0;
		}
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
        list<job>::iterator it_j = jobs->begin();
        for (it_j = jobs->begin() ; it_j != jobs->end() ; ++it_j)
 	        cout << "[" << (*it_j).id << "] " << (*it_j).cmd << " " 
                 << (*it_j).pid << " " << ((*it_j).is_running ? "(running)" : "(stopped)")  << endl;	
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
        pid_t pid = getpid();

        cout << "smash pid is " << pid << endl;    

	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{

        if (num_arg == 0)
        {
            if (fg_job == NULL) 
            {
				fprintf(stderr, "smash error: > no fg job\n");
                return 1;
            }
            fg_job->is_running  = true;
            fg_job->is_fg  = true;
            cout << fg_job->cmd << endl;
            fprintf(stdout, "smash > signal SIGCONT was sent to %d\n", fg_job->pid);
	        kill(fg_job->pid, SIGCONT);
            pause();
        }
        else
        {
            int num  = atoi(s_args[1].c_str());
            list<job>::iterator itr = find_job(num);
            if (itr != jobs->end())
            {
                (*itr).is_running  = true;
                (*itr).is_fg  = true;
                cout << (*itr).cmd << endl;
                fg_job = &(*itr);
                fprintf(stdout, "smash > signal SIGCONT was sent to %d\n", fg_job->pid);
                kill((*itr).pid, SIGCONT);
                pause();
            }
            else
				fprintf(stderr, "smash error: > \"%s\" - job doesn't exist\n", cmd);
        }
        return 0;
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{

        if (num_arg == 0)
        {
            if (fg_job == NULL)
            {
				fprintf(stderr, "smash error: > no bg job\n");
                return 1;
            }
            fg_job->is_running = true;
            fg_job->is_fg = false;
            cout << fg_job->cmd << endl;
            fprintf(stdout, "smash > signal SIGCONT was sent to %d\n", fg_job->pid);
	        kill(fg_job->pid, SIGCONT);
        }
        else
        {
            int num  = atoi(s_args[1].c_str());
            list<job>::iterator itr = find_job(num);
            if (itr != jobs->end())
            {
                (*itr).is_running  = true;
                (*itr).is_fg  = false;
                cout << (*itr).cmd << endl;
                fg_job = &(*itr);
                fprintf(stdout, "smash > signal SIGCONT was sent to %d\n", fg_job->pid);
                kill((*itr).pid, SIGCONT);
            }
            else
				fprintf(stderr, "smash error: > \"%s\" - job doesn't exist\n", cmd);
        }
        return 0;
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
        if (num_arg == 0)
        {
            shell_vars.clear();
            jobs->clear();
            exit(0);
        }
        if (s_args[1] == "kill")
        {
            pid_t pid;
            list<job>::iterator it, it1;
            time_t sec1;
            for( it = jobs->begin() ; it != jobs->end() ; it++)
            {
                pid = it->pid;
                sec1 = time(NULL);
	            kill(it->pid, SIGTERM);

                for ( ; ;)
                {
                    if ( time(NULL) - sec1 > 5)
                    {
                        break;
                    }
                }
                for( it1 = jobs->begin() ; it1 != jobs->end() ; it1++)
                {
                    if (pid == it1->pid)
                    {
	                    kill(pid, 9);
                        if (jobs->empty())
                            return 0;
                        break;
                    }
                }
            }
        }

    } 
	/*************************************************/
	else if (!strcmp(cmd, "set")) 
	{
        if (num_arg < 2)
        {
            fprintf(stderr, "smash error: > not enough arguments\n");
            return 1;
        }

        Vars tmp;
        string arg1(s_args[1]);
        string arg2(s_args[2]);
        tmp.key = arg1;
        tmp.data = arg2;
        it = find(shell_vars.begin(), shell_vars.end(), arg1);
        if (it != shell_vars.end())
            shell_vars.erase(it);

        shell_vars.push_back(tmp);
        return 0;
	} 
	/*************************************************/
	else if (!strcmp(cmd, "unset")) 
	{
        if (num_arg < 1)
        {
            fprintf(stderr, "smash error: > \"\" - variable not found\n"); // TODO: fix error message
            return 1;
        }

        string arg1(s_args[1]);
        it = find(shell_vars.begin(), shell_vars.end(), arg1);
        if (it != shell_vars.end())
        {
            shell_vars.erase(it);
            return 0;
        }
        else
        {
            fprintf(stderr, "smash error: > \"%s\" - variable not found\n", s_args[1].c_str()); 
            return 1;
        }
	} 
	/*************************************************/
    else if (!strcmp(cmd, "show"))
	{
        if (num_arg == 0)
        {   
            for ( it = shell_vars.begin() ; it != shell_vars.end() ; it++)
                cout << (*it).key << " := " << (*it).data << endl;
            return 0;
        }
        else
        {
            string x(s_args[1]);
            it = find(shell_vars.begin(), shell_vars.end(), x);
            if (it == shell_vars.end())
            {
                cout << "var doesnt exit" << endl;
                return 1;
            }
            else
            {
                cout << (*it).key << " := " << (*it).data << endl;
                return 0;
            }
	    }
    }
	/*************************************************/
    else if (!strcmp(cmd, "kill"))
    {
        if (num_arg < 2)
        {   
            fprintf(stderr, "smash error: > \"%s\" - not enough arguments\n", cmd); // TODO fix message
            return 1;
        }
        int sig_num = atoi(s_args[1].c_str() + 1);
        int job_number = atoi(s_args[2].c_str());
        list<job>::iterator it = find_job(job_number);

        if (it != jobs->end())
        {
            if(kill(it->pid, sig_num))
            {
                fprintf(stderr, "smash error: > kill job - cannot send signal\n"); // TODO fix message
                return 1;
            }
            else
                return 0;

        }
        else
        {
            fprintf(stderr, "smash error: > kill job - job does not exist\n"); // TODO fix message
            return 1;
        }


    }
	/*************************************************/
    else // external command
	{
		ExeExternal(args, cmd);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
 
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
    string cmd(cmdString);
	pid_t pID;
    switch(pID = fork()) 
	{
    		case -1: // frok failed
                fprintf(stderr, "smash error: > Failed to run the \"%s\"\n", cmdString); 
                break;
					
        	case 0 : // Child Process
               		setpgrp();
                    execvp(cmdString, args); // TODO: execv or execvp ?
                    exit(1);
                    break;
					 
			default: // Parent Process
                    new_job.id = 0;
                    new_job.cmd = cmd;
                    new_job.pid = pID;
                    new_job.is_running = true;
                    new_job.is_fg = true;
                    fg_job = &new_job;
                    pause();
			        break;		
					 
					
	}
}

//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************

int ExeComp(char* lineSize, bool bg)
{
	pid_t pID;
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    char arg1[MAX_LINE_SIZE] = "csh";
    char arg2[MAX_LINE_SIZE] = "-f";
    char arg3[MAX_LINE_SIZE] = "-c";
    char cmdString[MAX_LINE_SIZE];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || 
        (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {   
        strcpy(cmdString, lineSize);
		cmdString[strlen(lineSize)-1]='\0';
        args[0] = arg1;
        args[1] = arg2;
        args[2] = arg3;
        args[3] = cmdString;
        args[4] = NULL;

        switch(pID = fork()) 
	    {
    		case -1: // frok failed
                fprintf(stderr, "smash error: > Failed to run the \"%s\"\n", lineSize); 
                break;
					
        	case 0 : // Child Process
               		setpgrp();
                    execvp(*args, args); // TODO: execv or execvp ?
                    exit(0);
                    break;
					 
		    default: // Parent Process
                    new_job.id = 0;
                    new_job.cmd = cmdString;
                    new_job.pid = pID;
                    new_job.is_running = true;
                    fg_job = &new_job;
                    if (bg == false)
                    {
                        new_job.is_fg = true;
                        pause();
                    }
                    else
                    {
                        new_job.id = job_cnt++;
                        new_job.is_fg = false;
                        jobs->push_back(new_job);
                        fg_job = &(jobs->back());
                    }
			        break;		
	    }
	    return 0;
	} 
    return -1;
}

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************

int BgCmd(char* lineSize)
{
    pid_t pID;
	char* cmd;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
    int num_arg = 0;

	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';

        if(!ExeComp(lineSize, true))
            return 0;

        cmd = strtok(lineSize, delimiters);
	    if (cmd == NULL)
		    return 0; 
   	    args[0] = cmd;
	    for (int i=1; i<MAX_ARG; i++)
	    {
		    args[i] = strtok(NULL, delimiters); 
		    if (args[i] != NULL) 
			    num_arg++; 
	    }

        switch(pID = fork()) 
	    {
    		case -1: // frok failed
                fprintf(stderr, "smash error: > Failed to run the \"%s\"\n", cmd); 
                break;
					
        	case 0 : // Child Process
               		setpgrp();
                    execvp(cmd, args); // TODO: execv or execvp ?
                    exit(0);
                    break;
					 
			default: // Parent Process
                    new_job.id = job_cnt++;
                    new_job.cmd = cmd;
                    new_job.pid = pID;
                    new_job.is_running = true;
                    new_job.is_fg = false;
                    jobs->push_back(new_job);
                    fg_job = &(jobs->back());
			        break;	
        }
		
	    return 0;	
	}
	return -1;
}

