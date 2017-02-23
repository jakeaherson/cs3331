// ----------------------------------------------------------- 
// NAME : Jake Herson                   User ID: jaherson
// DUE DATE : 12/02/2016                                       
// PROGRAM ASSIGNMENT 5                                       
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :                                           
// 		Contains the function and class definitions for:
//			BoatThread
//			CannibalThread
//			MissionaryThread
//	as well as any necessary helper functions.
// ----------------------------------------------------------- 

#ifndef JAHERSON_THREAD_CPP
#define JAHERSON_THREAD_CPP

#include "ThreadClass.h"
#include "thread.h"
#include "boat-monitor.h"
#include <stdio.h>
#include <string.h>

// ----------------------------------------------------------- 
//	FUNCTION my_print:                          
//		A guarded call to cout. Preventing the buffer use by 
//		cout from race conditions.
//	PARAMETER USAGE :                                           
//		s: The string to print to stdout
//	FUNCTION CALLED :                                           
// 		Mutex::Lock()
// 		Mutex::Unlock()
// ----------------------------------------------------------- 
void my_printf(char *s){
	Print_Lock->Lock();
		cout << s << endl;
	Print_Lock->Unlock();
};

// ----------------------------------------------------------- 
// FUNCTION  BoatThread::BoatThread                        
//     BoatConstructor
// PARAMETER USAGE :                                           
//		n/a
// FUNCTION CALLED :                                           
//		n/a
// -----------------------------------------------------------
BoatThread::BoatThread(char* name, int max) : Thread(name), max(max) {
	pre = "*****";
};

// ----------------------------------------------------------- 
// FUNCTION  BoatThread::ThreadFunc                         
//    Boat's ThreadFunc, calls specified functions to determine
//		when a valid boat has been produced and then when that
// 		boat has been loaded.
// PARAMETER USAGE :                                           
//		n/a
// FUNCTION CALLED :                                           
//		Thread::Delay()
//		BoatMonitor::BoatReady()
//		BoatMonitor::BoatDone()
//		BoatMonitor::getValid()
//		BoatMonitor::getLoaded()
// -----------------------------------------------------------
void BoatThread::ThreadFunc(){
	char buf[200];
	char tmp[32];
	sprintf( buf, "%s BOAT thread begins\n", pre );
	my_printf( buf );
	while( mon->getCrossings() < max ){
		Thread::Delay();
		if( mon->getValid() ){
			sprintf( buf, "%s The boat is ready\n", pre );
			my_printf( buf );
			mon->BoatReady();
		}
		Thread::Delay();
		if( mon->getLoaded() ){
			sprintf( buf, "%s Boat load (%d): Passenger list (%s)\n",
					 pre, mon->getCrossings(), mon->getBoat() );
			my_printf( buf );
			mon->BoatDone();
		}
		Thread::Delay();
	}
	Exit();
};

// ----------------------------------------------------------- 
// FUNCTION  CannibalThread::CannibalThread                        
//     Cannibal Constructor
// PARAMETER USAGE :       
//		name: The name of the thread (cannibal+i)
//		id: int indentifier
// FUNCTION CALLED :                                           
//		n/a
// -----------------------------------------------------------
CannibalThread::CannibalThread(char* name, int id) : Thread(name),
	id(id) {
	pre = new char[100];
	for(int i = 0; i < id ; ++i) strcat(pre, " ");	
};

// ----------------------------------------------------------- 
// FUNCTION  CannibalThread::CannibalThread                        
//     Cannibal Constructor
//		Default constructor
// PARAMETER USAGE :                                           
//		n/a
// FUNCTION CALLED :                                           
//		n/a
// -----------------------------------------------------------
CannibalThread::CannibalThread() : Thread( NULL ), id(0) {};

// ----------------------------------------------------------- 
// FUNCTION  CannibalThread::~CannibalThread                        
//     Cannibal Destructor
// PARAMETER USAGE :                                           
//		n/a
// FUNCTION CALLED :                                           
//		n/a
// -----------------------------------------------------------
CannibalThread::~CannibalThread(){
	delete[] pre;
};

// ----------------------------------------------------------- 
// FUNCTION  CannibalThread::ThreadFunc                                                 
//    Cannibal ThreadFunc: Calls the *Arrives function to
//		attempt to get on the boat, sail across and then exit
//		without eating anyone.
// PARAMETER USAGE :                                           
//		n/a
// FUNCTION CALLED :                                           
//		Thread::Delay()
//		BoatMonitor::CannibalArrives()
// -----------------------------------------------------------
void CannibalThread::ThreadFunc(){
	char buf[200];
	char tmp[32];
	sprintf( buf, "%s Cannibal %d begins\n", pre, id );
	my_printf( buf );
	for( ;; ){
		Thread::Delay();
		/* enter the queue */
		sprintf( buf, "%s Cannibal %d arrives\n", pre, id );
		my_printf( buf );
		mon->CannibalArrives( id );
		Thread::Delay();
	}
};

// ----------------------------------------------------------- 
// FUNCTION  MissionaryThread::MissionaryThread                        
//     Missionary Constructor
// PARAMETER USAGE :  
//		name: the name of this thread (missionary+i)
//		id: int identifier
// FUNCTION CALLED :                                           
//		n/a
// -----------------------------------------------------------
MissionaryThread::MissionaryThread(char *name, int id) : Thread(name), 
	id(id) {
	pre = new char[100];
	for(int i = 0; i < id ; ++i) strcat(pre, " ");	
};

// ----------------------------------------------------------- 
// FUNCTION  MissionaryThread::MissionaryThread                        
//     Missionary Constructor
//		Default constructor.
// PARAMETER USAGE :                                           
//		n/a
// FUNCTION CALLED :                                           
//		n/a
// -----------------------------------------------------------
MissionaryThread::MissionaryThread() : Thread( NULL ), id(0) {};

// ----------------------------------------------------------- 
// FUNCTION  MissionaryThread::~MissionaryThread                        
//     Missionary Destructor
// PARAMETER USAGE :                                           
//		n/a
// FUNCTION CALLED :                                           
//		n/a
// -----------------------------------------------------------
MissionaryThread::~MissionaryThread(){
	delete[] pre;
};

// ----------------------------------------------------------- 
// FUNCTION  MissionaryThread::ThreadFunc                         
//    Missionary ThreadFunc: Calls the *Arrives function to
//		attempt to get on the boat, sail across and then exit
//		safely.
// PARAMETER USAGE :                                           
//		n/a
// FUNCTION CALLED :                                           
//		Thread::Delay()
//		BoatMonitor::MissionaryArrives()
// -----------------------------------------------------------
void MissionaryThread::ThreadFunc(){
	char buf[200];
	char tmp[32];
	sprintf( buf, "%s Missionary %d begins\n", pre, id );
	my_printf( buf );
	for( ;; ){
		Thread::Delay();
		/* enter the queue */
		sprintf( buf, "%s Missionary %d arrives\n", pre, id );
		my_printf( buf );
		mon->MissionaryArrives( id );
		Thread::Delay();
	}
};

#endif