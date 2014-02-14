#include "common.h"

extern FILE* FILE_SYSTEM_ID;

// This will return true if in range, false if out of range
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

void loadFloppyImage( char* location )
{
	FILE* file = fopen(location, "rb");

	if (file == NULL)
	{
		printf("Could not open the image.\n");
		gets(NULL);
		exit(1);
	}

	FILE_SYSTEM_ID = file;
	readBootSector();
}

char** parse( char input[], char** output)
{
	char* word = strtok(input, " ");
	int counter = 0;

	while (word) {
		output = realloc (output, sizeof (char*) * ++counter);

		if (output == NULL)
			exit (-1);

		output[counter - 1] = word;

		word = strtok (NULL, " ");
	}

	output = realloc (output, sizeof (char*) * (counter +1 ));
	output[counter] = 0;
	counter++;

	return output;
}

int countWords( const char input[] )
{
	int length = strlen(input);
	char* newInput = NULL;
	char* word;
	int counter = 0;

	newInput = strdup(input);

	word = strtok(newInput, " ");

	while (word != NULL) {
		++counter;

		word = strtok (NULL, " ");
	}

	return counter;
}

boolean equal(char* first, char* second)
{
	int length = strlen(first);
	int i;

	if(strcmp(first, second)  != 0)
	{
		return false;
	}

	for(i = 0; i < length; i++)
	{
		if(first[i] != second[i])
		{
			return false;
		}
	}

	return true;
}

void trimright( char input[] )
{
	int i=0;
	int front = 0;
	int endpos = 0;
	int len = strlen(input);
	char* newString;

	endpos = len - 1;

	/*while (isspace(input[i]))
	{
		front++;
	}*/

	while(isspace(input[endpos]))
	{
		endpos--;
	}
	input[endpos + 1] = '\0';

	//endpos += 1;

	//// now we a front and endpos
	//newString = (char*)malloc((endpos - front) * sizeof(char));
	//memset(newString, 0, strlen(newString));
	//strncpy(newString, input + front, (endpos - front));

	//len = strlen(newString);
	//newString[len] = 0;

	//input = (char*)malloc((strlen(newString) + 1) * sizeof(char));
	//memset(input, 0, strlen(input));
	//strncpy(input, newString, strlen(newString));

	//len = strlen(input);
	//input[len] = 0;

	//return newString;

	//input = newString;
}

void toLower( char* input )
{
	int i=0;
	char c;
	while (input[i])
	{
		c = input[i];
		input[i] = tolower(c);
		i++;
	}
}
