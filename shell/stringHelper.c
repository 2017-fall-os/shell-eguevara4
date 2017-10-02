#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* compare 2 strings */
int myStrCmp(char *a, char *b){
  while(*a && *b && *a == *b){
    a++;
    b++;
  }
  return (unsigned char) (*a) - (unsigned char) (*b);
}

/* find length of string */
int myStrLen(char *str){
  char *temp = str;
  int counter;
  for(counter = 0; *temp != 0; counter++){
    temp++;
  }
  return counter;
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
   Terminates with 0 */
char *subStr3(char *str, int offset, int size){
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
  
  *pCopy = 0;                      // terminate copy 
  return copy;
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
  
  *pCopy = '\n';                      // terminate copy 
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

/* returns number of pointers */
int sizeDP(char **dp){
  char **temp = dp;
  int count = 0;
  
  while(*temp != 0){
    count++;
    temp++;
  }
  return count;
}

/* prints pointers */
void printDP(char **dp){
  char **temp = dp;
  
  while(*temp != 0){
    printf("%s\n",*temp);
    temp++;
  }
}
