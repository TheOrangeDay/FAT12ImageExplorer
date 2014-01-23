#include "stdio.h"
#include "common.h"
#include "pfe.h"

void pfe(int x, int y)
{
	boolean validValues = false;

	validValues = checkRange(x, y);

	if(validValues)
	{
		printf("The values are valid!");
	}
}