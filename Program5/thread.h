// ----------------------------------------------------------- 
// NAME : Jake Herson                 User ID: jaherson
// DUE DATE : 12/02/2016                                       
// PROGRAM ASSIGNMENT 5                                       
// FILE NAME : thread.h
// PROGRAM PURPOSE :                                           
// 		Contains the headers and prototypes for the following
//		classes and functions:
//
//		BoatThread
//		CannibalThread
//		MissionaryThread
//		my_printf
// ----------------------------------------------------------- 

#ifndef JAHERSON_THREAD_H
#define JAHERSON_THREAD_H

#include "ThreadClass.h"
#include "boat-monitor.h"

extern Mutex *Print_Lock;
extern BoatMonitor *mon;

class BoatThread : public Thread {
	public:
		BoatThread(char*, int);
		//~BoatThread();
	private:
		void ThreadFunc();
		int max;
		char *pre;
};

class MissionaryThread : public Thread {
	public:
		MissionaryThread(char*, int);
		MissionaryThread();
		~MissionaryThread();
	private:
		void ThreadFunc();
		int id;
		char *pre;
};

class CannibalThread : public Thread {
	public:
		CannibalThread(char*, int);
		CannibalThread();
		~CannibalThread();
	private:
		void ThreadFunc();
		int id;
		char *pre;
};

void my_printf(char *);
#endif