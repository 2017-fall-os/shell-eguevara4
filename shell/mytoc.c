#include <stdio.h>
#include "mytoc.h"
#include "stdlib.h"

/* Returns a copy of a substring specified by offset and size */
char *subStr(char *str, int offset, int size){
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
  
  *pCopy = 0;			// terminate copy 
  return copy;
}


/* Returns number of Tokens in the input string */
int getNumberOfTokens(char *str, char delim){
  char *pStr;
  size_t numTokens = 0;
  char tokenF = 0; //Token found

  /* If at least one non-delimiter character 
     is found then count the token. Do not count
     other characters till a delimiter is seen.*/
  for(pStr = str; *pStr; pStr++){
    if(*pStr != '\n' && *pStr == delim){
      tokenF = 0;
    }else if(*pStr != '\n' && *pStr != delim && tokenF == 0){
      numTokens++;
      tokenF = 1;
    }
  }
  return numTokens;
}

/* Returns the double pointer with all the tokens */
char ** mytoc(char *str, char delim){
  char **tokenVec;
  char *pStr;
  size_t currentTokenLength = 0; //Length of the current token being scanned
  int currentToken = 0; //Index of current token being scanned
  int startIndex = -1; //Start index of the current token in the string input
  int currentIndex = 0; //Current index of the string input.

  tokenVec = (char **) calloc(getNumberOfTokens(str,delim)+1, sizeof(char*));
  
  for(pStr = str; *pStr; pStr++){
    if(*pStr != '\n' && *pStr != delim){ //Count the current token length
      if(startIndex == -1){ //If it is the start of token then keep index
	startIndex = currentIndex;
      }
      currentTokenLength++;
    }else if(currentTokenLength != 0){

      //Allocate, copy, and return substring containing only the current token
      tokenVec[currentToken++] = subStr(str, startIndex, currentTokenLength);

      //Reset length and start index for next token found.
      currentTokenLength = 0;
      startIndex = -1;
    }
    
    currentIndex++; //Move to index of next character
  }
  
  tokenVec[currentToken] = 0; //Terminate Vector

  return tokenVec;
}

