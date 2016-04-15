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


//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString)
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
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
        if (num_arg == 0)
        {
            shell_vars.clear();
            exit(0);
        }
        
        exit(0); // TODO: add support to "quit kill"

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
	int pID, status;
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
                    waitpid(pID, &status, 0); 
			        break;		
					 
					
	}
}

//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
/*
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		
		//your code
		
	} 
	return -1;
}
*/
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
/*
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		
		your code
		
		
	}
	return -1;
}
*/
