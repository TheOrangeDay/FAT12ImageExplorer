#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"

extern FILE* FILE_SYSTEM_ID;

int main(int argc, char* argv[])
{
	int firstValue;
	int secondValue;
	int shm_ID;
	void* pointer = NULL;

	if(argc != 3)
	{
		printf("Incorrect values! Ex: %s 2 8\n", argv[0]);
		return 1;
	}

	// take in the values
	firstValue = atoi(argv[1]);
	secondValue = atoi(argv[2]);

	// str to get the shared memory
	if ((shm_ID = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0444)) < 0 )
        {
                perror("Error getting SHM segment.");
                exit(-1);
        }
	
	// actually get the data
        if ((pointer = shmat(shm_ID, NULL, 0)) == NULL)
        {
                perror("Error including SHM address space.");
                exit(0);
        }

	shmStruct *value = (shmStruct*)pointer;

	loadFloppyImage(value->floppyName);

	// close shared memory
	if (shmdt(pointer) < 0)
	{
		perror("Error deallocating shared memory.");
		exit(-1);
	}

	pfe(firstValue, secondValue);

	return 0;
}
