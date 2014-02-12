#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "fatSupport.h"
#include "pbs.h"

typedef char boolean;
#define true  1
#define false 0

#define MAXPATHSIZE 512

char workingDirectoryName[MAXPATHSIZE];

boolean checkInRange(int x, int min, int max);
boolean checkRange(int x, int y);

boolean equal(char* first, char* second);
int countWords(const char input[]);
char** parse(char input[], char** output);
void toLower(char* input);
void trimright(char input[]);

void loadFloppyImage(char* location);

#endif
