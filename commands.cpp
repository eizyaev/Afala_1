//		commands.c
//********************************************
#include "commands.h"
#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <set>
#include <sys/stat.h>
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
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{

        getcwd(pwd, sizeof(pwd));
        if ((strcmp(args[1], "-") == 0))
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
        else if (chdir(args[1]) == 0)
        {
            strcpy(prev_pwd, pwd);
            return 0;
        }
        else
        {
            fprintf(stderr, "smash error: > \"%s\" not found\n", args[1]);
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
			fprintf(stderr, "smash error: > \"%s\" - cannot create directory\n", args[0]);
			return 1;
		}
		else
		{
			getcwd(pwd, sizeof(pwd));
			char pwd_helper[MAX_LINE_SIZE];
			strcpy(pwd_helper, pwd);
			strcat(pwd_helper, "/");
			strcat(pwd_helper, args[1]);

			if (chdir(pwd_helper) == 0)
			{
				chdir(pwd);
				fprintf(stderr, "smash error: > \"%s\" - directory already exists\n", args[0]);
                return 1;
			}

			if (mkdir(pwd_helper, S_IRWXU | S_IRGRP | S_IXGRP | S_IXOTH | S_IROTH ) != 0)
			{
				fprintf(stderr, "smash error: > \"%s\" - cannot create directory\n", args[0]);
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
   		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "set"))
	{
        Vars tmp;
        string arg1(args[1]);
        string arg2(args[2]);
        tmp.key = arg1;
        tmp.data = arg2;
        shell_vars.push_back(tmp);
        return 0;
	} 
	/*************************************************/
    else if (!strcmp(cmd, "show"))
	{
        list<Vars>::const_iterator i;
        if (num_arg == 0)
        {
            for ( i = shell_vars.begin() ; i != shell_vars.end() ; i++)
                cout << (*i).key << " := " << (*i).data << endl;
            return 0;
        }
        else
        {
            string x(args[1]);
            i = find(shell_vars.begin(), shell_vars.end(), x);
            if (i == shell_vars.end())
            {
                cout << "var doesnt exit" << endl;
                return 1;
            }
            else
            {
                cout << (*i).key << " := " << (*i).data << endl;
                return 0;
            }
	    }
    }
	/*************************************************/
    else // external command
	{
    //		ExeExternal(args, cmdString);
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
/* 
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					
					your code
					
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					 
					your code
					
			
			default:
                	// Add your code here
					
					 
					your code
					
	}
}
*/
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
