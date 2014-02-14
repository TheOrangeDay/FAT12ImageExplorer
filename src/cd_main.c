#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "fatSupport.h"

extern int BYTES_PER_SECTOR;
extern char workingDirectoryName[512];

int main(int argc, char* argv[])
{
	// parse current working dir, and find location on
	// fat table
	char garbage[64];

	char pwd[512];

	char* name;		 // name of the directory
	char* flcBuffer; // the FLC buffer
	char* newName;   // The new name buffer

	int mostSignificantBits;
	int leastSignificantBits;
	unsigned char* buffer;    // The buffer to pull data into
	int byteLocation = 19;    // the starting byte/sector location
	char* token;              // working token set
	char* newLocation;        // new location we want to cd to
	int currentDirectory = 0; // The current directory we are checking in
	int maxDirectories = 16;  // The max directories held per sector
	int i;
	int flcNumber;            // The final FLC number location
	int byteOffset = 0;		  // Current calculated byte offset for directories

	char path[] = "subdir1/subsub/sssub4"; //The path we want to navigate to
	int pathLength = sizeof(path)/sizeof(*path); // The length of the path
	int readSize = 0;
	// Temp Load
	loadFloppyImage("img/floppy2");

	//Set temporary directory
	strcpy(workingDirectoryName, "/");

	// set temp location

	newLocation = (char*) malloc(pathLength * sizeof(char));
	memset(newLocation, 0, strlen(newLocation));
	strcpy(newLocation, path);
	newLocation[pathLength] = 0;

	printf("PWD: %s\n", workingDirectoryName);

	// push into new string to avoid editing existing var
	strcpy(pwd, workingDirectoryName);

	// validate correct slashes are used eventually
	token = strtok(pwd, "/");

	printf("FirstToken: %s\n", token);

	while(token != NULL)
	{
		// each sector is 512, containing 16 directory entries (32 bytes long)
		BYTES_PER_SECTOR = 512;
		//size it up to 512
		buffer = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
		readSize = read_sector(byteLocation, buffer);
		
		for(i = 0; i < maxDirectories; i++)
		{
			byteOffset = i * 32;
			
			//check to see if we should ignore this.
			if(buffer[byteOffset] == 0xE5 || buffer[byteOffset + 11] == 0x02 || byteOffset > readSize)
			{
				// Deleted, not good
				printf("File Does Not Exist :(\n");
				continue;
			}

			printf("Directory is good thus far\n");

			flcBuffer = (char*) malloc(2 * sizeof(char));
			memset(flcBuffer, 0, strlen(flcBuffer));
			strncpy(flcBuffer, buffer + byteOffset + 26, 2);

			mostSignificantBits  = ( ( (int) flcBuffer[1] ) << 8 ) & 0x0000ff00;
			leastSignificantBits =   ( (int) flcBuffer[0] )        & 0x000000ff;
			flcNumber = mostSignificantBits | leastSignificantBits;
		
			printf("Begin name allocation\n");

			name = (char*) malloc(9 * sizeof(char));
			memset(name, 0, strlen(name));
			strncpy(name, buffer + byteOffset, 8);
			name[8] = 0;

			trimright(name);
			toLower(name);
		
			printf("Name allocation success\n");
			printf("Name: %s\n", name);
			/*if(strlen(flcBuffer) > 0)
			{
				flcNumber = flcBuffer[0];
			}
			else
			{
				//Refers to root. 
				flcNumber = 0;
			}*/
			printf("FLC: %i\n", flcNumber);

			if(equal(name, token))
			{
				//Found it! Find the FLC
				printf("MATCH: FLC: %i\n", flcNumber);

				//set new byte location here
				byteLocation = (flcNumber - 2 + 33);

				break;
			}
		}
		
		// get next token
		token = strtok(NULL, "/");
	}

	printf("Done. BL = %i\n", byteLocation);
	
	if(strlen(newLocation) > 0 && newLocation[0] == '/')
	{
		// set back to root
		byteLocation = 19;
	}

	token = strtok(newLocation, "/");

	while(token != NULL)
	{
		// each sector is 512, containing 16 directory entries (32 bytes long)
		BYTES_PER_SECTOR = 512;
		//size it up to 512
		buffer = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
		read_sector(byteLocation, buffer);

		for(i = 0; i < maxDirectories; i++)
		{
			byteOffset = i * 32;

			//check to see if we should ignore this.
			if(buffer[byteOffset] == 0xE5 || buffer[byteOffset + 11] == 0x02)
			{
				// Deleted, not good
				printf("File Does Not Exist :(\n");
				continue;
			}

			printf("Directory is good thus far\n");

			flcBuffer = (char*) malloc(2 * sizeof(char));
			memset(flcBuffer, 0, strlen(flcBuffer));
			strncpy(flcBuffer, buffer + byteOffset + 26, 2);

			mostSignificantBits  = ( ( (int) flcBuffer[1] ) << 8 ) & 0x0000ff00;
			leastSignificantBits =   ( (int) flcBuffer[0] )        & 0x000000ff;
			flcNumber = mostSignificantBits | leastSignificantBits;

			printf("Begin name allocation\n");

			name = (char*) malloc(9 * sizeof(char));
			memset(name, 0, strlen(name));
			strncpy(name, buffer + byteOffset, 8);
			name[8] = 0;

			trimright(name);
			toLower(name);

			printf("Name allocation success\n");
			printf("Name: %s\n", name);
			/*if(strlen(flcBuffer) > 0)
			{
				flcNumber = flcBuffer[0];
			}
			else
			{
				//Refers to root. 
				flcNumber = 0;
			}*/
			printf("FLC: %i\n", flcNumber);

			if(equal(name, token))
			{
				//Found it! Find the FLC
				printf("MATCH: FLC: %i\n", flcNumber);

				//set new byte location here
				byteLocation = (flcNumber - 2 + 33);

				// add to PWD the name
				strcat(pwd, "/");
				strcat(pwd, name);
				
				break;
			}
		}

		// get next token
		token = strtok(NULL, "/");
	}

	strcat(pwd, "/");

	printf("new dir: %s\n", pwd);

	gets(garbage);

	return 0;
}
