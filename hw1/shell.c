#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>

#define INPUT_STRING_SIZE 80

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"

int cmd_quit(tok_t arg[]) {
  printf("Bye\n");
  exit(0);
  return 1;
}

    int cmd_help(tok_t arg[]);

          int cmd_pwd(tok_t arg[]);
        int cmd_cd(tok_t arg[]);
       char *Backdirectory;

/* Command Lookup Table Structures */
typedef int cmd_fun_t (tok_t args[]); // cmd functions take token array and return int
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;
// add more commands to this lookup table!
fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_quit, "quit", "quit the command shell"},
{cmd_pwd, "pwd", "print the working directory"},
{cmd_cd, "cd", "change the directory"},
};

int lookup(char cmd[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

int cmd_help(tok_t arg[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

int cmd_pwd(tok_t arg[])
{
	char details[2500];
	char *cwd = getcwd(details,2500);
	printf("%s \n",cwd);
	return 1;
}


int cmd_cd(tok_t arg[])
{
	if(strcmp(arg[0], "-")== 0){
		
		chdir(Backdirectory);
		
	}
	else if(strcmp(arg[0],"~")==0){
		//rm -preserve-root
	Backdirectory = get_current_dir_name();
		chdir(getenv("HOME"));	
	}
	else{
	Backdirectory = get_current_dir_name();
		chdir(arg[0]);}
		
	
	return 1;
}


void init_shell()
{
  // Check if we are running interactively
  shell_terminal = STDIN_FILENO;

  // Note that we cannot take control of the terminal if the shell is not interactive
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){

    // force into foreground
    while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
      kill( - shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    // Put shell in its own process group
    if(setpgid(shell_pgid, shell_pgid) < 0){
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    // Take control of the terminal
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  
  /** YOUR CODE HERE */
  // ignore signals
}

/**
 * Add a process to our process list
 */
void add_process(process* p)
{
  /** YOUR CODE HERE */
}

/**
 * Creates a process given the tokens parsed from stdin
 *
 */
process* create_process(tok_t* tokens)
{
  /** YOUR CODE HERE */
  return NULL;
}



int shell (int argc, char *argv[]) {
  pid_t pid = getpid();		 // get current process's PID
  pid_t ppid = getppid();	 // get parent's PID
  pid_t cpid;                // use this to store a child PID

  char *s = malloc(INPUT_STRING_SIZE+1); // user input string
  tok_t *t;			                     // tokens parsed from input
  // if you look in parse.h, you'll see that tokens are just c-style strings 
  int lineNum = 0;
  int fundex = -1;
  
  // perform some initialisation
  init_shell();

  //printf("%s running as PID %d under %d\n",argv[0],pid,ppid);

  lineNum=0;
  int store;
  
  // change this to print the current working directory before each line as well as the line number.
  fprintf(stdout, "%d %s: ", lineNum,get_current_dir_name());
  while ((s = freadln(stdin))){
	lineNum= lineNum+1;
    t = getToks(s); // break the line into tokens
    fundex = lookup(t[0]); // Is first token a shell literal
    if(fundex >= 0) cmd_table[fundex].fun(&t[1]);
    else {
       
    	           int pid = fork();
    	               if(pid == 0) // child process
    	{	 
                               
                          int x = 0;
                          int y =0 ;
                          int z =0 ;				


	                                                           int check=0;
	                                                           char *current_out = dup(1);
	                                while(t[x]){
					  if(strchr(t[x],'>')!=NULL)
					    {
						char* filing = t[x+1];
						
						int input = open(filing, O_CREAT|O_TRUNC|O_RDWR, 0644);
						dup2(input,1);
						t[x]=0;
						t[x+1]=0;
						
					      }
					else if(strchr(t[x],'<')!=NULL)
					    {
						char* filed = t[x+1];
						
						int takingout = open(filed,O_RDONLY);
						dup2(takingout,0);
						
						t[x]=0;
						t[x+1]=0;
						
					    }
					x++;
				}
			
	
    			
    		if(strchr(t[0],'/') == NULL)
    		{
    			
    			                       char *dirtry = getenv("PATH");

					        char *head = strtok(dirtry,":");
				
				
					             while(head != NULL)

					               {
						      char *tmp = malloc(sizeof(char)*strlen(head)); 
						      for(int x=0;x<strlen(head);x++)
						     {
							tmp[x]= head[x];
						     }
						
						      strcat(tmp,"/");

						         strcat(tmp,t[0]);
						
					
						       if(execv(tmp,t) == 0)
						     {
							exit(1);
						     }
						
						head = strtok(NULL,":");
							
					}
					exit(1);
    		}
    		
    		else if(execv(t[0],t)==0)
    		{
    			exit(1);
    		}	 
    		
    	}
 			
    	else if(pid>0){wait(&store);}
    	else{printf("%s\n","fork error");}
		}	
    
		fprintf(stdout, "%d %s: ",lineNum,get_current_dir_name());
  }
  return 0;
}
