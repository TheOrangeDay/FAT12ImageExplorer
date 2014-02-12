CC = gcc
EXECUTABLE = shell pfe_main pbs_main cd_main pwd_main
#Add others when they are made

all: $(EXECUTABLE)

shell: shell.c
	$(CC) -o shell shell.c pbs.c fatSupport.c common.c

pfe_main: pfe_main.c
	$(CC) -o pfe pfe_main.c pfe.c pbs.c fatSupport.c common.c

pbs_main: pbs_main.c
	$(CC) -o pbs pbs_main.c pbs.c fatSupport.c common.c
	
cd_main: cd_main.c
	$(CC) -g -O -o cd cd_main.c pbs.c fatSupport.c common.c

pwd_main: pwd_main.c
	$(CC) -o pwd pwd_main.c pbs.c fatSupport.c common.c
