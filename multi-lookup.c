/*
 * File: multi-lookup.c
 * Author: Leslie Minor <minorl@colorado.edu>
 * Project: CSCI 3753 Programming Assignment 2
 * Create Date: 2014/02/25
 * Modify Date: 2012/02/28
 * Description:
 *	Multi-threaded DNS resolution  
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h> //for numCores

#include "multi-lookup.h"
#include "queue.h"
#include "util.h"

#define qSize 10

//Global vars
int openRequesters = 0;
pthread_mutex_t queueMutex;
pthread_mutex_t writeMutex;
pthread_mutex_t counterMutex;
pthread_cond_t 	queueNotFullCond = PTHREAD_COND_INITIALIZER;
queue q;


int main(int argc, char* argv[]){

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
	FILE* outputfp = NULL;
	int i; // y u no forloop initial declarations c?!
	int rc; //return code, thread creation
	int numResolverThreads;
	int numCPU = sysconf(_SC_NPROCESSORS_ONLN);

	numResolverThreads = numCPU >= MIN_RESOLVER_THREADS ? numCPU : MIN_RESOLVER_THREADS;
    pthread_t resolverThreads[numResolverThreads];

    /* Open Output File */
    outputfp = fopen(argv[(argc-1)], "w");
    if(!outputfp){
		fprintf(stderr, "Error Opening Output File\n");
		return EXIT_FAILURE;
    }

    //Initialize queue
    if(queue_init(&q, qSize) == QUEUE_FAILURE){
		fprintf(stderr,
			"error: queue_init failed!\n");
    }

    //Initialize mutexes (mutices?)
    if(pthread_mutex_init(&queueMutex, NULL) ||
    pthread_mutex_init(&writeMutex, NULL) ||
    pthread_mutex_init(&counterMutex, NULL)){
    	fprintf(stderr, "mutex init failed");
    	return EXIT_FAILURE;
    }

    /* Spawn Requester Threads */
    for(i=0; i< numReqesters; i++){
    	rc = pthread_create(&(requesterThreads[i]), NULL, RequestIP, &argv[i+1]);
		if (rc){
	    	fprintf(stderr, "ERROR; return code from pthread_create() is %d\n", rc);
	    	exit(EXIT_FAILURE);	
    	}
    }

    /* Spawn Resolver Threads */
    for(i=0; i<numResolverThreads; i++){
    	rc = pthread_create(&(resolverThreads[i]), NULL, ResolveName, (void*)&outputfp);
    	if (rc){
    		fprintf(stderr, "ERROR; return code from pthread_create() is %d\n", rc);
    		exit(EXIT_FAILURE);
    	}
    }

    //Clean up threads, waiting until everyone's done
    for(i=0; i< numReqesters; i++){
    	pthread_join(requesterThreads[i], NULL);
    }
    for(i=0; i< numResolverThreads; i++){
    	pthread_join(resolverThreads[i], NULL);

    }

    //Clean up!
    fclose(outputfp);
    queue_cleanup(&q);
    if(pthread_mutex_destroy(&queueMutex) ||
    pthread_mutex_destroy(&counterMutex) ||
    pthread_mutex_destroy(&writeMutex))
    	fprintf(stderr, "Mutex destroy fail\n");
    return EXIT_SUCCESS;
}

void* RequestIP(void* fd){
	FILE* inputfp;
	char** fname = fd;
	char hostname[SBUFSIZE];
	char* hostname_ptr = NULL;

	//Open file
    inputfp = fopen(*fname, "r");
    //given a bad file name, nothing for thread to do.
	if(!inputfp){
    	fprintf(stderr, "Error Opening Input File: %s\n", *fname);
    	pthread_mutex_lock(&counterMutex);
		openRequesters--;
		pthread_mutex_unlock(&counterMutex);
		return NULL;
    }

	while(fscanf(inputfp, INPUTFS, hostname) > 0){
			
		//push hostname on queue
		pthread_mutex_lock(&queueMutex);

		//if queue is full, wait until there's room
		while(queue_is_full(&q)){
			pthread_cond_wait(&queueNotFullCond, &queueMutex);
		}
		//check if queue is full, trust no one
		if (!queue_is_full(&q)) {
			hostname_ptr = malloc(sizeof(hostname));
			strcpy(hostname_ptr, hostname);
			if(queue_push(&q, hostname_ptr) == QUEUE_FAILURE){
				fprintf(stderr, "queue push fail \n");
			}
			hostname_ptr = NULL;
		}
		else{
			printf("done goofed, dropped a hostname \n");
		}
		pthread_mutex_unlock(&queueMutex);
	}

	fclose(inputfp);
	//decrement counter
	pthread_mutex_lock(&counterMutex);
	openRequesters--;
	pthread_mutex_unlock(&counterMutex);
	return NULL;
}

void* ResolveName(void* fd){
	FILE** outputfp = fd;
	int queueIsEmpty = 1;
	char* hostname_ptr = NULL;
	char firstipstr[MAX_IP_LENGTH];
	node* head_ptr;
	node* tmp_ptr;

	// while requesters are open and queue not empty
	while(openRequesters || !queueIsEmpty){
		//pop hostname
		pthread_mutex_lock(&queueMutex);
		//if queue was full, it's not anymore! tell all your friends
		pthread_cond_signal(&queueNotFullCond);
		//if the queue is not empty, popit
		if(!(queueIsEmpty = queue_is_empty(&q))){
			if((hostname_ptr = queue_pop(&q)) == NULL){
				fprintf(stderr, "error: queue_pop fail \n");
			}
		}
		else{
			pthread_mutex_unlock(&queueMutex);	
			continue;
		}

		pthread_mutex_unlock(&queueMutex);	

		head_ptr = malloc(sizeof(node));
		head_ptr->link = NULL;
		if(multidnslookup(hostname_ptr, head_ptr, sizeof(firstipstr)) 
			== UTIL_FAILURE){
			fprintf(stderr, "dnslookup error: %s\n", hostname_ptr);
			strncpy(firstipstr, "", sizeof(firstipstr));
		}

		pthread_mutex_lock(&writeMutex);

		/* Write to Output File */
		fprintf(*outputfp, "%s", hostname_ptr);
	    while(head_ptr->link != NULL){
	    	fprintf(*outputfp, ",%s", head_ptr->data);
	    	tmp_ptr = head_ptr;
			head_ptr = head_ptr->link;
			free(tmp_ptr);
	    }

	    fprintf(*outputfp, "\n");

	    //If you love it, set it free
	    free(head_ptr);
	    free(hostname_ptr);
		pthread_mutex_unlock(&writeMutex);
	}
	return NULL;
}
