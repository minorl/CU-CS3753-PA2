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
#include <unistd.h> //for sleep


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

    printf("spawning requester \n");
    /* Spawn Requester Threads */
    for(i=0; i< numReqesters; i++){
    	rc = pthread_create(&(requesterThreads[i]), NULL, RequestIP, &argv[i+1]);
		if (rc){
	    	printf("ERROR; return code from pthread_create() is %d\n", rc);
	    	exit(EXIT_FAILURE);	
    	}
    }
    printf("spawning resolvers \n");
    /* Spawn Resolver Threads */
    for(i=0; i<RESOLVER_THREADS; i++){
    	rc = pthread_create(&(resolverThreads[i]), NULL, ResolveName, (void*)&outputfp);
    	if (rc){
    		printf("ERROR; return code from pthread_create() is %d\n", rc);
    		exit(EXIT_FAILURE);
    	}
    }

	
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
    for(i=0; i< numReqesters; i++){
    	pthread_join(requesterThreads[i], NULL);
    }
    for(i=0; i< RESOLVER_THREADS; i++){
    	pthread_join(resolverThreads[i], NULL);

    }

    /* Close Output File */
    fclose(outputfp);
    queue_cleanup(&q);

    return EXIT_SUCCESS;
}

void* RequestIP(void* fd){
	FILE* inputfp;
	char** fname = fd;
	char hostname[SBUFSIZE];
	char* hostname_ptr = NULL;
	int queueIsFull = 0;

	//Open file
    inputfp = fopen(*fname, "r");
	if(!inputfp){
    	fprintf(stderr, "Error Opening Input File: %s", *fname);
    }
    printf("Opened: %s\n", *fname);

	while(fscanf(inputfp, INPUTFS, hostname) > 0){
		//if queue was full last time we tried, sleep a little
		if(queueIsFull) 
			usleep(rand()%100);
		//push hostname on queue
		pthread_mutex_lock(&queueMutex);
		//check if queue is full, trust no one
		if (!(queueIsFull=queue_is_full(&q))) {
			hostname_ptr = malloc(sizeof(hostname));
			strcpy(hostname_ptr, hostname);
			if(queue_push(&q, hostname_ptr) == QUEUE_FAILURE){
				fprintf(stderr, "queue push fail \n");
			}
			hostname_ptr = NULL;
		}
		pthread_mutex_unlock(&queueMutex);
	}
	printf("closed file %s \n", *fname);
	fclose(inputfp);

	//decrement counter
	pthread_mutex_lock(&counterMutex);
	openRequesters--;
	pthread_mutex_unlock(&counterMutex);
	printf("openrequesters: %d", openRequesters);

	return NULL;
}

void* ResolveName(void* fd){
	FILE* outputfp = fd;
	int queueIsEmpty = 1;

	// while requesters are open and queue not empty
	// while(openRequesters && !queue_is_empty(&q)){
	while(openRequesters && queueIsEmpty){
		//pop hostname
		pthread_mutex_lock(&queueMutex);
		//if the queue is not empty, popit
		if(!(queueIsEmpty = queue_is_empty(&q))){
			queue_pop(&q);
		}
		pthread_mutex_unlock(&queueMutex);		
		//resolve name
		//write to file
		pthread_mutex_lock(&writeMutex);

		pthread_mutex_unlock(&writeMutex);
	}
	printf("Job's done\n");

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

/* Questions
-Do requesters need to check if name has been resolved? (probably not)

*/