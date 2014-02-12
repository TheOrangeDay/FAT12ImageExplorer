#include <stdlib.h>
#include <stdio.h>
#include "common.h"

extern char workingDirectoryName[512];

int main(int argc, char* argv[])
{
	printf ("%s", workingDirectoryName);

	return 0;
}
