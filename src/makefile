CC = gcc
EXECUTABLE = shell pfe_main pbs_main
#Add others when they are made

all: $(EXECUTABLE)

shell: shell.c
	$(CC) -o shell shell.c pbs.c fatSupport.c common.c

pfe_main: pfe_main.c
	$(CC) -o pfe pfe_main.c pfe.c pbs.c fatSupport.c common.c

pbs_main: pbs_main.c
	$(CC) -o pbs pbs_main.c pbs.c fatSupport.c common.c
	
