#include "stdio.h"
#include "fatSupport.h"
#include "common.h"
#include "pfe.h"

void pfe(int x, int y)
{
	boolean validValues = false;
	unsigned char* buffer;
	int result;

	validValues = checkRange(x, y);

	if(validValues)
	{
		printf("The values are valid!\n");
	}

	buffer = (unsigned char*) malloc(512 * sizeof(unsigned char));

	readFAT12Table(0, buffer);

	result = get_fat_entry(3, buffer);

	printf("Result: %d", result);
}