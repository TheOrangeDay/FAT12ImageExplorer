#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"
#include "fatSupport.h"

extern int BYTES_PER_SECTOR;
extern char workingDirectoryName[512];

int main(int argc, char* argv[])
{
	// parse current working dir, and find location on
	// fat table

	int shm_ID;
	void* pointer = NULL;
	
	char pwd[512];

	char* name;		 // name of the directory
	char* flcBuffer; // the FLC buffer
	char* newName;   // The new name buffer
	int attribute;

	int mostSignificantBits;
	int leastSignificantBits;
	unsigned char* buffer;    // The buffer to pull data into
	unsigned char* fatTable;  // this is the fat table
	int byteLocation = 19;    // the starting byte/sector location
	char* token;              // working token set
	char* newLocation;        // new location we want to cd to
	int currentDirectory = 0; // The current directory we are checking in
	int maxDirectories = 16;  // The max directories held per sector
	int i;
	int flcNumber;            // The final FLC number location
	int lastFLC = 0;
	int byteOffset = 0;		  // Current calculated byte offset for directories
	boolean found = false;

	char path[512];// = "course/eccs/eccs46"; //The path we want to navigate to
	strcpy(path, argv[1]);
	printf("Path: %s\n", path);
	int pathLength = sizeof(path)/sizeof(*path); // The length of the path
	int readSize = 0;

	if ((shm_ID = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0444)) < 0 )
        {
                perror("Error getting SHM segment.");
                exit(-1);
        }

        if ((pointer = shmat(shm_ID, NULL, 0)) == NULL)
        {
                perror("Error including SHM address space.");
                exit(0);
        }

	shmStruct *value = (shmStruct*)pointer;

	// Temp Load
	loadFloppyImage(value->floppyName);

	// set temp location
	newLocation = (char*) malloc(pathLength * sizeof(char));
	memset(newLocation, 0, strlen(newLocation));
	strcpy(newLocation, path);
	newLocation[pathLength] = 0;

	// fill in the pwd from parent
	strcpy(workingDirectoryName, value->pwd);
	printf("PWD from Main: %s\n", workingDirectoryName);

	// deallocate pointer
	if (shmdt(pointer) < 0)
	{
		perror("Error deallocating shared memory.");
		exit(-1);
	}

	// push into new string to avoid editing existing var
	if(strlen(workingDirectoryName) > 1)
	{
		strcpy(pwd, workingDirectoryName);
	}
	else
	{
		pwd[0] = 0;
	}

	if(equal(path, "/"))
        {
                workingDirectoryName[1] = 0;
                workingDirectoryName[0] = '/';
        }
	else
	{

	// validate correct slashes are used eventually
	token = strtok(pwd, "/");

	printf("FirstToken: %s\n", token);

	BYTES_PER_SECTOR = 512;

	//Read in the fat table:
	fatTable = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
	readFAT12Table(0, fatTable);

	while(token != NULL)
	{
		// each sector is 512, containing 16 directory entries (32 bytes long)
		//size it up to 512
		buffer = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));
		readSize = read_sector(byteLocation, buffer);
		found = false;

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

			if(equal(name, token))
			{
				//Found it! Find the FLC
				printf("MATCH: FLC: %i\n", flcNumber);
				lastFLC = flcNumber;

				found = true;

				//set new byte location here
				byteLocation = (flcNumber - 2 + 33);

				break;
			}
		}

		if (found == false)
		{
			int flc = get_fat_entry(lastFLC, fatTable);
			if(flc != 4095)
			{
				byteLocation = flc + 31;

				printf("Next SECTOR!!\n");

				// skip the token at the end and try again
				continue;
			}
			else
			{
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
		found = false;

		for(i = 0; i < maxDirectories; i++)
		{
			byteOffset = i * 32;

			attribute = buffer[byteOffset + 11];

			//check to see if we should ignore this.
			if(buffer[byteOffset] == 0xE5 || buffer[byteOffset + 11] == 0x02
				|| !(attribute & 16))
			{
				// Deleted, not good, skip it
				continue;
			}
			
			flcBuffer = (char*) malloc(2 * sizeof(char));
			memset(flcBuffer, 0, strlen(flcBuffer));
			strncpy(flcBuffer, buffer + byteOffset + 26, 2);

			mostSignificantBits  = ( ( (int) flcBuffer[1] ) << 8 ) & 0x0000ff00;
			leastSignificantBits =   ( (int) flcBuffer[0] )        & 0x000000ff;
			flcNumber = mostSignificantBits | leastSignificantBits;

			name = (char*) malloc(9 * sizeof(char));
			memset(name, 0, strlen(name));
			strncpy(name, buffer + byteOffset, 8);
			name[8] = 0;

			trimright(name);
			toLower(name);

			printf("Name: %s\n", name);

			if(equal(name, token))
			{
				//Found it! Find the FLC
				printf("MATCH: FLC: %i\n", flcNumber);

				lastFLC = flcNumber;

				//set new byte location here
				byteLocation = (flcNumber - 2 + 33);

				found = true;

				if(equal(name, ".."))
				{
					char* last = strrchr(pwd, '/');
					int index = last - pwd;
					pwd[index] = 0;
				}
				else if(!equal(name, "."))
				{
					// add to PWD the name
					strcat(pwd, "/");
					strcat(pwd, name);
				}
				
				break;
			}
		}

		if (found == false)
		{
			int flc = get_fat_entry(lastFLC, fatTable);
			if(flc != 4095)
			{
				byteLocation = flc + 31;

				printf("Next SECTOR!!\n");

				// skip the token at the end and try again
				continue;
			}
			else
			{
				break;
			}
		}

		// get next token
		token = strtok(NULL, "/");
	}

	printf("new dir: %s\n", pwd);

	strcpy(workingDirectoryName, pwd);
	}

	shm_ID = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666);
	pointer = shmat(shm_ID, NULL, 0);

	shmStruct newStruct;
	strcpy(newStruct.pwd, workingDirectoryName);

	memset(pointer, 0, 0);
	memcpy(pointer, &newStruct, sizeof(newStruct));

	shmdt(pointer);

	return 0;
}
