#include "common.h"

/* This will return true if in range, false if out of range
 */
boolean checkInRange(int x, int min, int max)
{
	boolean passed = false;
	if((x > min && x <= max))
	{
		passed = true;
	}

	return passed;
}

boolean checkRange(int x, int y)
{
	boolean passed = false;
	if(x <= y && x <= 2)
	{
		passed = true;
	}

	return passed;
}