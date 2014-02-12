#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include "common.h"

int main(int argc, char *argv[])
{
	char buffer[512];
	char* workingBuffer;
	char** result = NULL;
	int word_count, i;

	pid_t pid, id = getpid();

	if (getpid() == id)
	{
		// continue until we exit
		while(1)
		{
			//print the prompt
			printf("DocOS> ");
			
			//get input
			fgets(buffer, 512, stdin);

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
			if(equal(first, "exit"))
			{
				printf("Goodbye\n");
				return 0;
			}

			//fork!
			pid = fork();

			//Error checking to see if fork works
			if (pid != 0)
			{
				// wait for child process to finish
				wait(pid);
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
				
				if ((dir = opendir("./")) != NULL)
				{
					boolean found = false;
					while ((ent = readdir (dir)) != NULL)
					{
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

