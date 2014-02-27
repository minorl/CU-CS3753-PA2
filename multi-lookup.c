/*
 * File: multi-lookup.c
 * Author: Leslie Minor
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

#include "util.h"

#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"
 //limits from writeup
#define MAX_INPUT_FILES 10
#define MAX_RESOLVER_THREADS 10
#define MIN_RESOLVER_THREADS 2
#define MAX_NAME_LENGTH 1025
#define MAX_IP_LENGTH INET6_ADDRSTRLEN

int main(int argc, char* argv[]){

/*	create struct with mutex for queue and mutex and fd of file to write to
	for thread function

	two thread functions 
		one for putting names in queue & checking resolution [requesters]
		one for resolving names and putting in results.txt
	
	#requester threads = #name files
	
	requester thread reads name from file, attempts to write to queue.
	if full sleep

	resolver threads
	pop and query, write to results.txt
	hostname,ipaddr

	------

	open files
	create struct
	create thread pools
	
*/

 //    /* Local Vars */
 //    FILE* inputfp = NULL;
 //    FILE* outputfp = NULL;
 //    char hostname[SBUFSIZE];
 //    char errorstr[SBUFSIZE];
 //    char firstipstr[INET6_ADDRSTRLEN];
 //    int i;
    
 //    /* Check Arguments */
 //    if(argc < MINARGS){
	// fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
	// fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
	// return EXIT_FAILURE;
 //    }

 //    /* Open Output File */
 //    outputfp = fopen(argv[(argc-1)], "w");
 //    if(!outputfp){
	// perror("Error Opening Output File");
	// return EXIT_FAILURE;
 //    }

 //    /* Loop Through Input Files */
 //    for(i=1; i<(argc-1); i++){
	
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

 //    /* Close Output File */
 //    fclose(outputfp);

    return EXIT_SUCCESS;
}