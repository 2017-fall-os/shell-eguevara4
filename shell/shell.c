#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytoc.h"
#include "stringHelper.h"

/* forks off a child to execute the command given by str */
int execCommand(char *str,char **paths, char **envp){
  char **command;
  char delimCommand = ' ';
  int waitStatus = 0;
  
  command = mytoc(str, delimCommand); //tokenize command
  if(command[0] == 0){
    return -1;
  }

  pid_t forkVal = fork();

  if(forkVal == -1){
    printf("Error fork()");
  }
    
  if(forkVal == 0){
    int index = 0;
    int retVal;
      
    retVal = execve(command[0], &command[0], envp); //Try command as is
    while(paths[index] != 0){ //Look through tokenized paths and try to exec
      char *p = strConcat(paths[index],command[0]); //Concatinate path to command namr
      retVal = execve(p, &command[0], envp);
      index++;
    }
    //No paths in PATH were able to execute the command
    fprintf(stderr,"%s: command not found...\n",command[0]);
    exit(2);
  }else{
    //fprintf(stderr,"Waiting command: %s",str);
    waitpid(forkVal, &waitStatus, 0);
    //fprintf(stderr,"Return command %s",str);
  }
  free(command);
  return 0;
}

/* uses pipe to execute command c1 and direct the output to command c2 */
int pipeCommands(char *c1, char *c2,char **paths, char **envp){
  int pip[2];
  int waitStatus = 0;

  pid_t pipeProcPID = fork();

  if(pipeProcPID == 0){
    pipe(pip);
    pid_t forkVal = fork();

    if(forkVal == -1){
      printf("Error fork()");
    }

    if(forkVal == 0){
      close(1);
      dup(pip[1]);
      close(pip[0]);
      close(pip[1]);

      int size = 0;
      char *command = c1;
      for(size = 0; command[size] != 0; size++);
      char *firstCommand = subStr2(c1,0,size);
      int retVal = execCommand(firstCommand,paths,envp);
      exit(2);
    }else{
      char buf[1024];
      close(0);
      dup(pip[0]);
      close(pip[0]);
      close(pip[1]);

      int size = 0;
      char *command = c2;
      for(size = 0; command[size] != 0; size++);
      char *secondCommand = subStr2(c2,0,size);
      int retVal = execCommand(secondCommand,paths,envp);
      exit(2);   
    }
  }else{
    //fprintf(stderr,"Waiting command: %s",str);
    waitpid(pipeProcPID, &waitStatus, 0);
    return 0;
    //fprintf(stderr,"Return command %s",str);
  }
}

int main(int argc, char **argv, char **envp){

  char str[1024];
  char delimPipe = '|';
  char delimPaths = ':';
  char delimCommand = ' ';
  char **pipeC;
  char **paths;
  char **tokens;
  char *pwd;
  
  int i = 0;
  int j = 0;

  while(envp[i] != 0){ // Look through envp
    if(strCmp(envp[i],"PATH=",5) == 0){ // Find PATH=
      char *str = envp[i];
      int n = myStrLen(str);
      char *pathsString = subStr2(envp[i],5,n-5); // Trim PATH= from paths string
      paths = mytoc(pathsString, delimPaths); // Tokenize paths
    }
    if(strCmp(envp[i],"PWD=",4) == 0) { // Find PWD=
      char *pwdfull = envp[i];
      int n = myStrLen(pwdfull);
      pwd = subStr3(envp[i],4,n-4);
    }
    i++;
  }

  write(0,"Shell started.\n",15);
  
  while(1){
    int c;
    for(c = 0; c < 1024; c++){ // Clear str buffer
      str[c] = 0;
    }

    write(0,"$ ",2);
    read(0, str, 1024);

    if(myStrCmp(str,"exit\n") == 0){ //If input == exit
      write(0,"Goodbye!\n",9);
      return 0;
    }
    
    tokens = mytoc(str,delimCommand);

    int tokensSize = sizeDP(tokens);
    char *lastToken = tokens[tokensSize-1];

    /* Check to run in background if so then run in child and do not wait for it to finish*/
    if(tokensSize > 1 && strCmp(lastToken,"&",myStrLen(lastToken)) == 0){
      int amperIndex;
      for(amperIndex = 0; str[amperIndex] != '&'; amperIndex++);
      char *trimmedStr = subStr2(str,0,amperIndex);

      pipeC = mytoc(trimmedStr,delimPipe);
      int size = sizeDP(pipeC);

      pid_t forkVal = fork();

      if(forkVal == -1){
	printf("Error fork()");
      }

      if(forkVal == 0){
        if(size > 1 ){// Pipe command
	  pipeCommands(pipeC[0],pipeC[1],paths,envp);
	}
	else if(strCmp(tokens[0],"cd",myStrLen(tokens[0])) == 0){ // chdir command
	  int cdIndex;
	  for(cdIndex = 0; str[cdIndex] != 'c' && str[cdIndex] != 'd'; cdIndex++);
	  char *trimmedCommand = subStr2(str, cdIndex+3, myStrLen(str)-cdIndex+3);
	  int i = 0;
	  while(trimmedCommand[i] != 0){
	    if(trimmedCommand[i] == '\n'){
	      trimmedCommand[i] = 0;
	    }
	    i++;
	  }
	  char *path = strConcat(pwd,trimmedCommand);
	  int success = chdir(path);
	  if(success == 0){
	    pwd = strConcat(pwd,trimmedCommand);
	  }else{
	    printf("No such file or directory\n");
	  }
	}
	else{// Normal command
	  int retCode = execCommand(trimmedStr,paths,envp);
	  if(retCode == -1){
	    printf("Empty Command\n");
	  }
	}
        exit(0);
      }else{
	printf("Process: %d Handling command: %s",forkVal, trimmedStr);
	//Continue shell;
      }
      
    }
    else{ // Do not run in background
      pipeC = mytoc(str,delimPipe);
      int size = sizeDP(pipeC);

      if(size > 1 ){// Pipe command
	pipeCommands(pipeC[0],pipeC[1],paths,envp);
      }
      else if(strCmp(tokens[0],"cd",myStrLen(tokens[0])) == 0){// chdir command
	int cdIndex;
	for(cdIndex = 0; str[cdIndex] != 'c' && str[cdIndex] != 'd'; cdIndex++);
	char *trimmedCommand = subStr2(str, cdIndex+3, myStrLen(str)-cdIndex+3);
	int i = 0;
	while(trimmedCommand[i] != 0){
	  if(trimmedCommand[i] == '\n'){
	    trimmedCommand[i] = 0;
	  }
	  i++;
	}
	char *path = strConcat(pwd,trimmedCommand);
	int success = chdir(path);
	if(success == 0){
	  pwd = strConcat(pwd,trimmedCommand);
	}else{
	  printf("No such file or directory\n");
	}
      }
      else{// Normal command
	int retCode = execCommand(str,paths,envp);
	if(retCode == -1){
	  printf("Empty Command\n");
	}
      }
    }
  }
  
  return 0;
  
}
