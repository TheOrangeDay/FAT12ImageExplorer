#include <stdio.h>
#include <string.h>

extern FILE* FILE_SYSTEM_ID;

int main(int argc, char* argv)
{
	// take in the values
	char garbage[64];
	int firstValue = atoi("2");
	int secondValue = atoi("8");

	loadFloppyImage("img/floppy1");

	pfe(2, 8);

	gets(garbage);

	return 0;
}