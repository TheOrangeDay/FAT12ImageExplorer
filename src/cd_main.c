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
	char* name; // name of the directory
	char* newName;
	char* flcBuffer;
	unsigned char* buffer;
	int byteLocation = 19;
	char* token;
	char* newLocation;
	char hello;
	int currentDirectory = 0;
	int maxDirectories = 16;
	int i;
	int flcNumber;
	int byteOffset = 0;

	loadFloppyImage("img/floppy2");

	printf("loaded\n");

	//Set temporary directory
	strcpy(workingDirectoryName, "subdir1/");
	newLocation = (char*) malloc(7 * sizeof(char));
	memset(newLocation, 0, strlen(newLocation));
	strcpy(newLocation, "subsub");
	newLocation[6] = 0;

	printf("PWD: %s\n", workingDirectoryName);

	// validate correct slashes are used eventually
	token = strtok(workingDirectoryName, "/");

	printf("FirstToken: %s\n", token);
	// "subdir"

	while(token != NULL)
	{
		// find current token byte location (search for "subdir")
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
		
			printf("Begin name allocation\n");

			name = (char*) malloc(8 * sizeof(char));
			memset(name, 0, strlen(name));
			strncpy(name, buffer + byteOffset, 8);

			name = trim(name);
			toLower(name);
		
			printf("Name allocation success\n");
			printf("Name: %s\n", name);
			if(strlen(flcBuffer) > 0)
			{
				flcNumber = flcBuffer[0];
			}
			else
			{
				//Reefers to root. 
				flcNumber = 0;
			}
			printf("FLC: %i\n", flcNumber);

			if(equal(name, token))
			{
				//Found it! Find the FLC
				printf("MATCH: FLC: %i\n", flcBuffer);
				//set new byte location here
				byteLocation = (flcNumber - 2 + 33);

				break;
			}
		}
		
		// get next token
		token = strtok(NULL, "/");
	}

	printf("Done. BL = %i\n", byteLocation);

	// lets rock on, muh-fuhh uhh

	//while(token != NULL)
	//{
		// find current token byte location (search for "subdir")
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

			printf("Begin name allocation\n");

			name = (char*) malloc(8 * sizeof(char));
			memset(name, 0, strlen(name));
			strncpy(name, buffer + byteOffset, 8);

			name = trim(name);
			toLower(name);

			printf("Name allocation success\n");
			printf("Name: %s\n", name);
			if(strlen(flcBuffer) > 0)
			{
				flcNumber = flcBuffer[0];
			}
			else
			{
				//Reefers to root. 
				flcNumber = 0;
			}
			printf("FLC: %i\n", flcNumber);

			if(equal(name, newLocation))
			{
				//Found it! Find the FLC
				printf("MATCH: FLC: %i\n", flcBuffer);
				//set new byte location here
				byteLocation = (flcNumber - 2 + 33);
				// set PWD To str8 baller mode including name
				// 
				strcat(workingDirectoryName, "/");
				strcat(workingDirectoryName, name);
				strcat(workingDirectoryName, "/");

				break;
			}
		}

		// get next token
		//token = strtok(NULL, "/");
	//}

	printf("new dir: %s\n", workingDirectoryName);

	gets(garbage);

	return 0;
}
