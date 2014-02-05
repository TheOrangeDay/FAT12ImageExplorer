#include "pbs.h"
#include "common.h"

extern FILE* FILE_SYSTEM_ID;

int main(int argc, char* argv[])
{
	char garbage[64];

	if(argc != 2)
	{
		printf("Incorrect values! Ex: %s floppy1\n", argv[0]);
		return 1;
	}	

	loadFloppyImage(argv[1]);

	printBootSector();

	gets(garbage);

	return 0;
}
