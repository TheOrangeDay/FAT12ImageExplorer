#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include "fatSupport.h"
#include "pbs.h"

typedef char boolean;
#define true  1
#define false 0

boolean checkInRange(int x, int min, int max);
boolean checkRange(int x, int y);

void loadFloppyImage(char* location);

#endif