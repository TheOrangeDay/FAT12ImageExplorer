#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <sys/shm.h>
#include "common.h"

extern char workingDirectoryName[512];
extern FILE* FILE_SYSTEM_ID;

int main(int argc, char *argv[])
{
	char buffer[512];
	char* workingBuffer;
	char** result = NULL;
	int word_count, i;
	
	int shm_ID;
	void* pointer = NULL;
	
	strcpy(workingDirectoryName, "/");

	pid_t pid, id = getpid();
	
	// load the flip flop image
	loadFloppyImage(argv[1]);

	if (getpid() == id)
	{
		// continue until we exit
		while(1)
		{
			//print the prompt
			printf("DocOS> ");
			
			//get input
			fgets(buffer, 512, stdin);
			
			printf("PWD: %s\n", workingDirectoryName);

			// clear input of new line
			int len = strlen(buffer);
			if( buffer[len-1] == '\n' )
			{
				buffer[len-1] = 0;
			}

			// add to new pointer of char to not destroy buffer
			workingBuffer = malloc(strlen(buffer));
			strcpy(workingBuffer, buffer);

			char* first = strtok(workingBuffer, " ");

			//check if we want to exit
			if(equal(first, "exit") || equal(first, "quit") || equal(first, "logout"))
			{
				printf("Goodbye\n");
				return 0;
			}
			
			shm_ID = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666);
			pointer = shmat(shm_ID, NULL, 0);
			shmStruct newStruct;
			strcpy(newStruct.pwd, workingDirectoryName);
			strcpy(newStruct.floppyName, argv[1]);

			memcpy(pointer, &newStruct, sizeof(newStruct) + 1);
			
			shmdt(pointer);

			//fork!
			pid = fork();

			//Error checking to see if fork works
			if (pid != 0)
			{
				// wait for child process to finish
				wait(pid);

				printf("Child finished \n");

				// lets pull some data from the shared memory
				shm_ID = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0444);
				pointer = shmat(shm_ID, NULL, 0);

				shmStruct* receivedStruct;
				receivedStruct = (shmStruct*)pointer;

				// this should be from the cd function
				// so we need to get the new pwd from them
				strcpy(workingDirectoryName, receivedStruct->pwd);
				printf("New working directory: %s\n", workingDirectoryName);

				// deallocate that pointer because we don't need it anymore
				shmdt(pointer);
			}
			else
			{
				word_count = countWords(buffer);
				// must add null at end
				word_count++;
				
				// put all arguments into char**
				result = parse(&buffer, result);
				
				// loop through current directory
				DIR* dir;
				struct dirent *ent;
				int rv = 0;

				// open the one directory we are in
				if ((dir = opendir("./")) != NULL)
				{
					boolean found = false;
					while ((ent = readdir (dir)) != NULL)
					{
						// Is this the command we typed in?
						if(equal(result[0], ent->d_name))
						{
							found = true;
							result[0] = ent->d_name;
							rv = execv(ent->d_name, result);
							if(rv == -1)
							{
								printf("ERROR: %s\n", strerror(errno));
								exit;
							}
							
							break;
						}
					}
					
					if(found == false)
					{
						printf("Command does not exist!\n");
						exit;
					}
					
					closedir (dir);
				}
			}
		}
	}	
	return 0;
}

