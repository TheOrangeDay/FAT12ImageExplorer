#include "stdio.h"
#include "fatSupport.h"
#include "common.h"
#include "pfe.h"

extern int BYTES_PER_SECTOR;

void pfe(int x, int y)
{
	boolean validValues = false;
	unsigned char* buffer;
	int result;
	int i;

	validValues = checkRange(x, y);

	if(!validValues)
	{
		printf("The values are out of range!\n");
		exit(1);
	}

	buffer = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));

	readFAT12Table(0, buffer);

	for(i = x + 1; i <= y; i++)
	{
		result = get_fat_entry(i, buffer);
		printf("Entry %d: %X\n", i, result);
	}

	
}