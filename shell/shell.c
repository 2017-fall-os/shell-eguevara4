#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytoc.h"

/* compare 2 strings */
int myStrCmp(char *a, char *b){
  while(*a && *b && *a == *b){
    a++;
    b++;
  }
  return (unsigned char) (*a) - (unsigned char) (*b);
}

/* compare strings up to a certain point n */
int strCmp(char *a, char *b, int n){
  int i;
  int cmp = 0;
  for(i = 0; i < n; i++){
    if(a[i] != b[i]){
      cmp++;
    }
  }
  return cmp;
}

/* Returns a copy of a substring specified by offset and size 
   Terminates with new line character */
char *subStr2(char *str, int offset, int size){
  char *pStr, *copy, *pCopy;
  size_t len;
  int i = 0;
  pStr = str;
  len = size + 1; //size of substring plus terminator
  
  copy = pCopy = (char *)malloc(len); // allocate memory to hold  copy 

  for (pStr += offset; i < size; pStr++){ // duplicate 
    *(pCopy++) = *pStr;
    i++;
  }
  
  *pCopy = '\n';
  //*pCopy = 0;                             // terminate copy 
  return copy;
}

/* Concatinates two string. It adds a '/' character in the middle
   used to create paths to the actual command */
char *strConcat(char *str1, char *str2){
  char *copy;
  int i = 0;
  int j = 0;
  int str1len = 0;
  int str2len = 0;
  while(str1[str1len] != 0){ 
    str1len++;
  }
  while(str2[str2len] != 0){
    str2len++;
  }
  int index = 0;
  copy = (char *) malloc(str1len+str2len+2);
  while(index < str1len){ // Copy first str1
    copy[index++] = str1[i++];
  }
  copy[index++] = '/'; // Concatinate '/'
  while(index < str1len+str2len+1){ // Concatinate str2 
    copy[index++] = str2[j++];
  }
  copy[index] = 0;
  return copy;
}

int main(int argc, char **argv, char **envp){

  char str[1024];
  char delimCommand = ' ';
  char delimPaths = ':';
  int waitStatus = 0;
  char **command;
  char **paths;

  int i = 0;
  int j = 0;

  while(envp[i] != 0){ // Look through envp
    if(strCmp(envp[i],"PATH=",5) == 0){ // Find PATH=
      int n = 0;
      char *str = envp[i];
      while(str[n] != 0){ // Count length of PATH string
        n++;
      }
      char *pathsString = subStr2(envp[i],5,n-5); // Trim PATH= from paths string
      paths = mytoc(pathsString, delimPaths); // Tokenize paths
      break;
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
    
    command = mytoc(str, delimCommand); //tokenize command
    
    if(command[0] == 0){
      continue;
    }

    pid_t forkVal = fork();

    if(forkVal == 0){
      
      int index = 0;
      int retVal;
      
      retVal = execve(command[0], &command[0], envp); //Try command as is

      while(paths[index] != 0){ //Look through tokenized paths and try to exec
	char *p = strConcat(paths[index],command[0]); //Concatinate path to command name
	retVal = execve(p, &command[0], envp);
        index++;
      }
      //No paths in PATH were able to execute the command
      fprintf(stderr, "Command not found\n", command[0], retVal);
      return 0;
    }else{
      
      waitpid(forkVal, &waitStatus, 0);
    }
    
  }
  
  return 0;
}
