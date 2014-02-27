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

#ifndef QUEUE_H
#define QUEUE_H

#include "util.h" // for INET6_ADDRSTRLEN


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

#endif