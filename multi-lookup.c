/*
 * File: multi-lookup.c
 * Author: Leslie Minor <minorl@colorado.edu>
 * Project: CSCI 3753 Programming Assignment 2
 * Create Date: 2014/02/25
 * Modify Date: 2012/02/25
 * Description:
 *	Multi-threaded DNS resolution  
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "queue.h"
#include "util.h"
#include "multi-lookup.h"

#define RESOLVER_THREADS 5
#define qSize 20

//Global vars
int openRequesters = 0;

pthread_mutex_t queueMutex;
pthread_mutex_t writeMutex;
pthread_mutex_t counterMutex;
queue q;

int main(int argc, char* argv[]){

    /* Local Vars */
    // FILE* inputfp = NULL;
    // char hostname[SBUFSIZE];
    // char errorstr[SBUFSIZE];
    // char firstipstr[INET6_ADDRSTRLEN];
    // int i;

    /* Check Arguments */
    if(argc < MINARGS){
	fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
	fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
	return EXIT_FAILURE;
    }

    // Local Vars (after check args so no negative arrays)
    int numReqesters = argc-2;
    pthread_t requesterThreads[numReqesters];
    openRequesters = numReqesters;
    pthread_t resolverThreads[RESOLVER_THREADS];
	FILE* outputfp = NULL;
	FILE* inputfp = NULL;
	int i; // y u no forloop initial declarations c?!
	char errorstr[SBUFSIZE];
	int rc; //return code, thread creation


    /* Open Output File */
    outputfp = fopen(argv[(argc-1)], "w");
    if(!outputfp){
	perror("Error Opening Output File");
	return EXIT_FAILURE;
    }

    //Initialize queue
    if(queue_init(&q, qSize) == QUEUE_FAILURE){
	fprintf(stderr,
		"error: queue_init failed!\n");
    }


    /* Spawn Requester Threads */
    for(i=0; i< numReqesters-1; i++){
    	inputfp = fopen(argv[i+1], "r");
		if(!inputfp){
	    	sprintf(errorstr, "Error Opening Input File: %s", argv[i]);
	    	perror(errorstr);
	    	break;
	    }
    	rc = pthread_create(&(requesterThreads[i]), NULL, RequestIP, (void*)&inputfp);
		if (rc){
	    	printf("ERROR; return code from pthread_create() is %d\n", rc);
	    	exit(EXIT_FAILURE);	
    	}
    }

    /* Spawn Resolver Threads */
    for(i=0; i<RESOLVER_THREADS -1; i++){
    	rc = pthread_create(&(resolverThreads[i]), NULL, ResolveName, (void*)&outputfp);
    	if (rc){
    		printf("ERROR; return code from pthread_create() is %d\n", rc);
    		exit(EXIT_FAILURE);
    	}
    }
	
	// /* Open Input File */
	// inputfp = fopen(argv[i], "r");
	// if(!inputfp){
	//     sprintf(errorstr, "Error Opening Input File: %s", argv[i]);
	//     perror(errorstr);
	//     break;
	// }	

	// /* Read File and Process*/
	// while(fscanf(inputfp, INPUTFS, hostname) > 0){
	
	//     /* Lookup hostname and get IP string */
	//     if(dnslookup(hostname, firstipstr, sizeof(firstipstr))
	//        == UTIL_FAILURE){
	// 	fprintf(stderr, "dnslookup error: %s\n", hostname);
	// 	strncpy(firstipstr, "", sizeof(firstipstr));
	//     }
	
	//     /* Write to Output File */
	//     fprintf(outputfp, "%s,%s\n", hostname, firstipstr);
	// }

	// /* Close Input File */
	// fclose(inputfp);
 //    }

    /* Close Output File */
    fclose(outputfp);
    queue_cleanup(&q);

    return EXIT_SUCCESS;
}

void* RequestIP(void* fd){
	FILE* inputfp = fd;
	char hostname[SBUFSIZE];

	while(fscanf(inputfp, INPUTFS, hostname) > 0){
		//push hostname on queue
	}
	fclose(inputfp);
		//decrement counter
	return NULL;
}

void* ResolveName(void* fd){
	FILE* outputfp = fd;


	return NULL;
}

/*	
	global:
		mutex queue
		mutex write file
		mutex counter
		counter
		write file

	thread pools:
		request threads
		resolver threads

	two thread functions 
		one for putting names in queue [requesters]
		one for resolving names and putting in results.txt
	
	#requester threads = #name files
	
	requester thread reads name from file, attempts to add to queue.
	if full sleep

	resolver threads
	pop and query, write to results.txt
	if empty & counter !0, sleep
	hostname,ipaddr

	------

	open write file
	set counter 
	create threads
	
	decrement counter as requesters & join
	join resolvers
	
*/