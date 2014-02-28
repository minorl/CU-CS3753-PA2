/*
 * File: multi-lookup.h
 * Author: Leslie Minor <minorl@colorado.edu>
 * Project: CSCI 3753 Programming Assignment 2
 * Create Date: 2014/02/27
 * Modify Date: 2014/02/27
 * Description:
 * 	This is the header file for an implemenation of 
 *	the multi-threaded DNS resolution 
 */

#ifndef MULTILOOKUP_H
#define MULTILOOKUP_H

#include "util.h" // for INET6_ADDRSTRLEN


#define MINARGS 3
#define USAGE "<inputFilePath1> <inputFilePath2> ... <inputFilePathN> <outputFilePath>"
#define SBUFSIZE 1025 //max name length
#define INPUTFS "%1024s" //max name length

 //limits from writeup
#define MIN_RESOLVER_THREADS 2
#define MAX_IP_LENGTH INET6_ADDRSTRLEN

//Where fd is name of a file with hostnames
//Push hostnames on a queue to be resolved
void* RequestIP(void* fd);

//Where fd is a file pointer to a file 
//opened to be written to.
//Resolves hostnames from queue, writes
//output to file
void* ResolveName(void* fd);

#endif