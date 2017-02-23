// ----------------------------------------------------------- 
// NAME : Jake Herson                  User ID: jaherson
// DUE DATE : 12/02/2016                                       
// PROGRAM ASSIGNMENT 5                                       
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :                                           
// 		Main driver. Creates all threads and then joins with
//			the boat thread when all crossings complete.
// ----------------------------------------------------------- 

#ifndef JAHERSON_THREAD_MAIN
#define JAHERSON_THREAD_MAIN

#include "ThreadClass.h"
#include "boat-monitor.h"
#include "thread.h"
#include <stdio.h>

#define DEF_CANNIBAL 8
#define DEF_MISSIONARY 8
#define DEF_BOAT 5

Mutex *Print_Lock;
BoatMonitor *mon;

int main( int argc, char** argv ){
	if( argc < 4 ){
		cerr << "too few args" << endl;
		exit(1);
	}
	
	Print_Lock = new Mutex("print");
	int c,m,b;
	
	if( atoi( argv[1] ) == 0 )
			c = DEF_CANNIBAL;
	else c = atoi( argv[1] );
	
	if( atoi( argv[2] ) == 0 )
			m = DEF_MISSIONARY;
	else m = atoi( argv[2] );
	
	if( atoi( argv[3] ) == 0 )
			b = DEF_BOAT;
	else b = atoi( argv[3] );
	
	mon = new BoatMonitor(b);
	BoatThread *B = new BoatThread("boat-thread", b);
	
	B->Begin();
	
	CannibalThread **C = new CannibalThread*[c]();
	for( int i = 0; i < c; ++i ){
		char buf[20];
		sprintf( buf, "cannibal %d", i);
		C[i] = new CannibalThread(buf, i);
		C[i]->Begin();
	}
	
	MissionaryThread **M = new MissionaryThread*[m]();
	for( int i = 0; i < m; ++i ){
		char buf[20];
		sprintf( buf, "missionary %d", i);
		M[i] = new MissionaryThread(buf, i);
		M[i]->Begin();
	}
	
	B->Join();
	exit(0);
}

#endif