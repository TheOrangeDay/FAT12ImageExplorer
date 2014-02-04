#include "pbs.h"
#include "common.h"

extern FILE* FILE_SYSTEM_ID;

int main(int argc, char* argv)
{
	char garbage[64];

	loadFloppyImage(argv);

	printBootSector();

	gets(garbage);

	return 0;
}