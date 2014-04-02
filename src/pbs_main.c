#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "pbs.h"
#include "common.h"

extern FILE* FILE_SYSTEM_ID;

int main(int argc, char* argv[])
{
	void* pointer = NULL;
	int shm_ID;
	
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
	//memcpy(&value, pointer, sizeof(pointer));

	printf("pointer size: %i\n", sizeof(pointer));

	loadFloppyImage(value->floppyName);
	
	if (shmdt(pointer) < 0) 
	{ 
		perror("Error deallocating shared memory."); 
		exit(-1); 
	}
	
	readBootSector();

	printBootSector();

	return 0;
}
