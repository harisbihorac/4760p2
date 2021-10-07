/*
 * This file is the license obj. It handles all the functions stated in config.h. All messages are appended to file logfile.data.
 */

#include "config.h"

// Initialize object
int initlicense() {
	shm->flag = 0;

	printf("number of licenses available: %d\n", shm->availableLicenses);

	return shm->childProcess;
}

int getlicense() {
	if((shm->procs >= shm->availableLicenses) || shm->availableLicenses == 1) {
		shm->flag = 1;  // flag until one becomes available
	} else {
		shm->flag = 0;
	}

	return shm->flag;
}

// Increment licenses available
int returnlicense() {
	shm->childProcess--;
	return shm->childProcess;
}

// Add n licenses to the number available
void addtolicenses(int n) {
	shm->childProcess - n;
	shm->procs--;
}

// Decrements the number of licenses by n
void removelicenses(int n) {
	
	shm->childProcess + n;
}

// Log the messages to output file
void logmsg(char *pid, char *c, char *repeat) {
	FILE *ofptr;

	// Open outfile
	if((ofptr = fopen("logfile.data", "a")) == NULL) {
		perror("runsim: Error: ");
		exit(1);
	}
	
	// Append time
	addTime(ofptr);
	
	// Print msg to file
	fprintf(ofptr, "%s %s of %s\n", pid, c, repeat);

	// Close outfile
	fclose(ofptr);
}

// Add current time
void addTime(FILE *ofptr) {
	time_t tm;
	time(&tm);
	struct tm *tp = localtime(&tm);

	fprintf(ofptr, "%.2d:%.2d:%.2d ", tp->tm_hour, tp->tm_min, tp->tm_sec);
}
