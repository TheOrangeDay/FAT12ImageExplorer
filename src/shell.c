#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "common.h"

int main(int argc, char *argv[])
{
	//buffer is to hold the commands that the user will type in
	char buffer[512];
	// /bin/program_name is the arguments to pass to execv
	//if we want to run ls, "/bin/ls" is required to be passed to execv()
	char* path = "/bin/";
	char* workingBuffer;
	char** result = NULL;
	int word_count, i;

	pid_t pid, id = getpid();
	printf("BaseID: %d\n", id);
	
	char* argu[1];
	char flop[] = "floppy1";
	argu[0] = flop;

	int rv = execvp("./pbs", argu);

	printf("Return val: %i\n", rv);
	printf("ERROR: %s\n", strerror(errno));

	if(getpid() == id)
	{

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

			// add to new pointer of char
			workingBuffer = malloc(strlen(buffer));
			strcpy(workingBuffer, buffer);

			//count_words(workingBuffer);

			//print detected input
			printf("Input: %s\n", workingBuffer);

			char* first = strtok(workingBuffer, " ");

			if(equal(first, "exit"))
			{
				printf("Goodbye\n");
				return 0;
			}

			//fork!
			pid = fork();

			printf("Piss id: %d", pid);

			//Error checking to see if fork works
			//If pid !=0 then it's the parent
			if (pid != 0)
			{
				printf("MRR\n");
				wait(NULL);
			}
			else
			{
				printf("inner problem\n");
			
				word_count = countWords(buffer);
				result = parse(&buffer, result);
				// must add null at end
				word_count++;

				for (i = 0; i < word_count; ++i)
					printf ("result[%d] = %s\n", i, result[i]);

				char* argu[1];
				char flop[] = "floppy1";
				argu[0] = flop;

				int rv = execvp("pbs", argu);
			}
		}

	}

	if(pid != 0)
	{
		printf("OK OK\n");
	}
	else
	{
		//if pid = 0 then we're at teh child
		printf("FO real WILD CARD, BITCHES\n"); 
			
		/*//Count the number of arguments
		int num_of_args = count_words(buffer);
		printf("WILD CARD, BITCHES: %f\n", num_of_args);                        

		//create an array of pointers for the arguments to be passed to execcv.
		char *arguments[num_of_args+1];

		//parse the input and arguments will have all the arguments to be passed to the program
		parse(buffer, num_of_args, arguments);

		//set the last pointer in the array to NULL. Requirement of execv
		arguments[num_of_args] = NULL;

		//This will be the final path to the program that we will pass to execv
		char prog[512];
		//First we copy a /bin/ to prog
		strcpy(prog, path);
		//Then we concancate the program name to /bin/
		//If the program name is ls, then it'll be /bin/ls
		strcat(prog, arguments[0]);
		//pass the prepared arguments to execv and we're done!
		int rv = execv(prog, arguments);*/
	}
	
	return 0;
}

