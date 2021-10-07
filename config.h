/*
 * Header file for runsim and testsim -- contains function prototypes for license obj.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>

extern int sid;
extern struct nOfLicenses *shm;

struct nOfLicenses {
	int availableLicenses;	// Total number of licenses
	int childProcess;		// Number of child procs
	int procs;		// Number of procs currentlyrunning
	int choice[100];	// For bakery algorithm
	int turnNumber[100];	// Also for bakery algorithm
	int flag; 		// Flag for if all available liceses are being used
};

void addTime(FILE *ofptr);
int childCheck(pid_t child[], int size);
void signalHandler();
void killprocs();

extern int getlicense();
extern int returnlicense();
extern int initlicense();
extern void addtolicenses(int n);
extern void removelicenses(int n);
extern void logmsg(char* pid, char *c, char *repeat);

#endif
