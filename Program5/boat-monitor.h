// ----------------------------------------------------------- 
// NAME : Jake Herson                    User ID: jaherson
// DUE DATE : 12/02/2016                                       
// PROGRAM ASSIGNMENT 5                                       
// FILE NAME :boat-monitor.h
// PROGRAM PURPOSE :                                           
// 		Contains class definition for the boat monitor
// ----------------------------------------------------------- 

#ifndef JAHERSON_BOAT_MON_H
#define JAHERSON_BOAT_MON_H

#include "ThreadClass.h"
#define CANNIBAL 1
#define MISSIONARY 0

#define toDigit(c) (c-'0')

class BoatMonitor : public Monitor{
	public:
		BoatMonitor(int);
		~BoatMonitor();
		
		/* helpers */
		int getCrossings();
		int getValid();
		int getLoaded();
		char* getBoat();
		
		/* key functions */
		void CannibalArrives(int);
		void MissionaryArrives(int);
		void BoatReady();
		void BoatDone();
		
	private:	
		/* conditions */
		Condition *MQueue;
		Condition *CQueue;
		Condition *GetOff; // (my lawn!)
		
		/* counters, etc */
		int MQueue_Count;
		int CQueue_Count;
		int max_crossings;
		int cur_cross;
		int sailing;
		int p_count;
		int valid_boat;
		int boat_loaded;
		int *pass;
		int *pass2;
};

#endif