#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/times.h>
#include <stdio.h>


#define BOILERPLATE "Welcome to the JASH Shell.\n"\
          "Author: Joshua Rodstein\n"\
          "Spring 2017 - University of Pittsburgh\n"\
          "Department of Computer Science\n"
#define BUFFERSIZE 1024
#define DELIMETERS " ()<>|&;\n"


// prototypes
int exec_args(char**, char **); 
int run_args(char**, char **);


int main(){
  struct tms start_time, end_time;                                // tms structs for holding time stamp data
  char *input = malloc(sizeof(char*) * BUFFERSIZE);               // initial malloc of input buffer - size 1024 Max chars
  char *token;                                                    // token holds char * after strtok call
  char **token_array;                                             // token array hold parsed input args
  char **rd_array = malloc(sizeof(char *)*3);                     // rd_array hold in/out redirection file args
  char *rd;                                                       // rd holds strtok'd file arg 
  int i, timer;                                                   // i: counter for size of parsed args
                                                                  // timer: boolean value to trigger timestamp
  
  // print BOILERPLATE info upon shell start
  // check for successful allocation of input buffer. 
  // exit with code 1 if malloc unsuccessful
  printf("%s", BOILERPLATE);  
  if (input == NULL){
    perror("Buffer Allocation Error: ");;
    exit(1);
  }

  /*--- Shell Loop---*/
  while(1){
    // Initialize rd_array, timer and i at start of every loop
    rd_array[0] = NULL;
    rd_array[1] = NULL;
    timer = 0;
    i = 0;

    // input prompt -- reads using fgets and stdin
    printf("JASH $ ");
    fgets(input, BUFFERSIZE, stdin);
    
    /*Check for redirection chars using strstr
     rd holds pointer to the found char, or rd hold NULL if
     char not found. if found, strtok rd to remove white space 
     chars specified by DELIMETER macro. Store a '>' in rd_array[0]
     and '<' in rd_array[1]. The order of this parsing allows for
     double redirection. ie." sort < file.txt > sortedfile.txt "*/
    rd = strstr(input, ">");
    if(rd != NULL){
      rd_array[0] = strtok(rd, DELIMETERS);
    }
    rd = strstr(input, "<");
    if(rd != NULL){
      rd_array[1] = strtok(rd, DELIMETERS);
    }
    // place null character at end of tokenized args
    rd_array[2] = NULL;
    
    /*Allocate memory for array of tokenized args, of max size 'input'
      + 1 (for null terminator) and parse/store first argument into token */
    token_array = malloc(sizeof(char*)*strlen(input)+1);
    token = strtok(input, DELIMETERS);
    
    /*To allow for implementation of 'time' command, check if first tokenized
      arg is "time". If found, update boolean timer value to 1.. strtok next
      argument so that time is not passed as arg[0]. if the token is null, no
      other args were enters... re-prompt.*/
    if (strcmp(token, "time") == 0){
      token = strtok(NULL, DELIMETERS);
      timer = 1;
    }
    if (token == NULL) {
      continue;
    } 

    /*While loop to tokenize remaining arguments and place into arg array. check for
      matching rd_array file args. If found, parse, but do not store in the argument
      array, and do not increment array size 'i'*/
    while(token != NULL){
	if (rd_array[0] != NULL && strcmp(rd_array[0], token) == 0){
	  token = strtok(NULL, DELIMETERS);
	  continue;
	} else if (rd_array[1] != NULL && strcmp(rd_array[1], token) == 0){
	  token = strtok(NULL, DELIMETERS);
	  continue;
	}
      token_array[i] = token;
      token = strtok(NULL, DELIMETERS);
      i++;
    }
    // place null character at end of tokenized args.  
    token_array[i] = NULL;
   
    
    /*If timer has been set to 1, then call times() and store timestamp
      info into struct start_time
      pass tokenized program/command args and any tokenized redirection args
      to the exec_args function
      Upon function return, time stmp info into end_time, and print elapsed user
      and system time as double of precision 3. divide by configured clock tikcs per 
      second for the kernel */
      if(timer == 1){
	times(&start_time);
      }
      if(exec_args(token_array, rd_array) == 0){
	// if 0 returned, program exits successfully
	break;
     }
      if(timer == 1){
	times(&end_time);
	printf("\n\nUser : %.3lfs\n", (float)(end_time.tms_cutime - start_time.tms_cutime)/sysconf(_SC_CLK_TCK));
	printf("System : %.3lfs\n", (float)(end_time.tms_cstime - start_time.tms_cstime)/sysconf(_SC_CLK_TCK));
      }

      // after succesful execution w/o a exit, free the tokenized argument array
      free(token_array);
    
  } // Loop End
 
  // free input buffer before exit
  free(rd_array);
  free(token_array);
  free(input);
  exit(0);
}

/*---exec_args() BULT-IN shell command function---*/
int exec_args(char ** t, char ** d){
 
  /* If user enters exit, logoff, or quit.. shell will exit with 0*/
  if(strcmp(t[0], "exit") == 0
     || strcmp(t[0], "quit") == 0
     || strcmp(t[0], "logout") == 0){
    // call built in function for exit
    exit(0);
  } 

  /*cd command will call chdir() and pass specified path
    If Path invalid, perror() will print correspongding error 
    and return 1 to reprompt. if no args passed, will treat like '..'
    if '~' passed as arg, calles getenv() to retrvieve useres home 
    directoty file path */
  if (strcmp(t[0], "cd") == 0){
    // call built in function for chdir
    if(t[1] == NULL){
      if(chdir("..") == -1){
	perror("-JASH");
      } 
    } else if (strcmp(t[1], "~") == 0) {
      if(chdir(getenv("HOME")) == -1){
	perror("-JASH");
      }
    } else if (chdir(t[1]) == -1){
      perror("-JASH");
    }
    return 1;
  } 
  
  /*Help prints README documentation, returns 1 to reprompt*/
  if(strcmp(t[0], "help") == 0){
    printf("\n%s\n Help Documentation Coming soon!\n\n", BOILERPLATE);
    return 1;
  }   

  /*If not built in commands entered, pass both arg arrays to run_args function
    and create new process. */
  return run_args(t, d);
}

/*--- run_args() function creates a child process ---*/
int run_args(char ** t, char ** d){
  /* status holds exit status of child, pid holds result of
    child process creation */
  int status;
  pid_t pid;
  
  // fork() process to create exact duplicate of parent
  pid = fork();

  /* if pid is < 0 then print corresponding error and return with 1.
     which  will cause shell to reprompt */
  if(pid == -1) {
    perror("-JASH");
    return 1;
  } 
  /* if pid == 0, thenfork succesful, check redirection args for value. 
     if not null then < or > is present. freopen with read std in or w stdout
     and pass corresponding redirection file args. any error from fre0pen is printed,
     and exit is called with arg 1. this will signal a reprompt, and end the child process.  */
  if (pid == 0) {
    if(d[0] != NULL) {
      if (freopen(d[0], "w", stdout) == NULL){
	perror("-JASH");
	exit(1);
      }
    }

    if(d[1] != NULL) {
      if (freopen(d[1], "r", stdin) == NULL){
	perror("-JASH");
       	exit(1);
      }
    }
    /*replace identical child process with specified program in t[0], and pass
      the arg option in t. if returns < 0, print corresponding error. this will fall
      through to  exit with 1 and reprompt. */
    if(execvp(t[0], t) == -1){
      perror("-JASH");
    }

    exit(1);

  } else {
    wait(&status);
  }
 
  /* falling through to this exit with signal a reprompt after child process is terminated succesfully */ 
    return 1;
}


