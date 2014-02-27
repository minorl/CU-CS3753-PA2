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

#include "util.h"
#include "multi-lookup.h"


int main(int argc, char* argv[]){

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