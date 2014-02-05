#include <stdio.h>
#include <string.h>

extern FILE* FILE_SYSTEM_ID;

int main(int argc, char* argv[])
{
	char garbage[64];
	int firstValue;
	int secondValue;

	if(argc != 4)
	{
		printf("Incorrect values! Ex: %s floppy1 2 8\n", argv[0]);
		return 1;
	}

	// take in the values
	firstValue = atoi(argv[2]);
	secondValue = atoi(argv[3]);

	loadFloppyImage(argv[1]);

	pfe(firstValue, secondValue);

	gets(garbage);

	return 0;
}
