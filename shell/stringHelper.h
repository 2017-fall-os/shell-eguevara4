#ifndef StringHelperIncluded
#define StringHelperIncluded

int myStrCmp(char *a, char *b);

int strCmp(char *a, char *b, int n);

char *subStr2(char *str, int offset, int size);

char *subStr3(char *str, int offset, int size);

char *strConcat(char *str1, char *str2);

int sizeDP(char **dp);

void printDP(char **dp);

int myStrLen(char* str);

#endif
