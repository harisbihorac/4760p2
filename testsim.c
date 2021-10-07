/*
 * This file retrieves the shared memory initiated in runsim and then calls logmsg to print the messages the number of times stated in the repeat factor. The program also sleeps in between
 * each message loop as stated in the sleep time. The bakery algorithm is utlizied to handle the multiple procs created by fork in runsim.  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

int sid;
struct nOfLicenses *shm;

int main(int argc, char* argv[]) {
	signal(SIGINT, signalHandler);
	
	int sleepT = atoi(argv[1]);
	int repeatF = atoi(argv[2]);
	int i = atoi(argv[3]);
	key_t key = 5678;

	
	// Get shared memory id from parent
	if((sid = shmget(key, sizeof(struct nOfLicenses) * 2, 0666)) < 0) {
		perror("testsim: Error: shmget ");
		exit(1);
	}

	// Attach shared memory to child
	if((shm = (struct nOfLicenses *)shmat(sid, NULL, 0)) == (struct nOfLicenses *) -1) {
		perror("testsim: Error: shmat ");
		exit(1);
	}

	int c, j, k, n = shm->childProcess;
	int max = 0;
	shm->choice[(n-1)] = 1;

	for(c = 0 ; c < repeatF ; c++) {
		if((shm->turnNumber[c]) > max) {
			max = (shm->turnNumber[c]);
		}
		shm->turnNumber[(n-1)] = 1 + max;

		shm->choice[(n-1)] = 0;
		for(j = 0 ; j < n ; j++) {
			while(shm->choice[j] == 1) {}
				while((shm->turnNumber[j] != 0) && (shm->turnNumber[j] < shm->turnNumber[(n-1)])) {}
		}

		// Critical section
		pid_t id = getpid();
		char pid[50];
		char num[50];
	
		sprintf(pid, "%d", id);
		sprintf(num, "%d", (c + 1));

		printf("Printing msg to file: %s %s of %s\n", pid, num, argv[2]);

		// Print to output fie using logmsg
		logmsg(pid, num, argv[2]);

		// Sleep before looping again
		sleep(sleepT);
		
		// Exit critical section
		shm->turnNumber[(n-1)] = 0;

	}

	//shm->procs--;
	addtolicenses(1);
	killprocs();
	return 0;
}

void signalHandler() {
	pid_t id = getpid();
	killprocs();
	killpg(id, SIGINT);
	exit(1);
}

void killprocs() {
	shmdt(shm);
}
