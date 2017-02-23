// ----------------------------------------------------------- 
// NAME : Jake Herson                    User ID: jaherson
// DUE DATE : 12/02/2015                                       
// PROGRAM ASSIGNMENT 5                                       
// FILE NAME : boat-monitor.cpp
// PROGRAM PURPOSE :                                           
// 		Contains all the function definitions for the
//		boat monitor.
// ----------------------------------------------------------- 

#ifndef JAHERSON_BOAT_MON_CPP
#define JAHERSON_BOAT_MON_CPP

#include "ThreadClass.h"
#include "boat-monitor.h"
#include "thread.h"
#include <stdio.h>
#include <string.h>

// ----------------------------------------------------------- 
// FUNCTION BoatMonitor()                        
//     BoatMonitor constructor
// PARAMETER USAGE :       
//		m: max number of crossings
// FUNCTION CALLED :                                           
//		Monitor::Monitor(name, type)
//		Condition::Condition(name)
// -----------------------------------------------------------
BoatMonitor::BoatMonitor(int m) : Monitor("boat-mon", HOARE),
	max_crossings(m){
	
	MQueue = new Condition("mq");
	CQueue = new Condition("cq");
	GetOff = new Condition("get off my lawn!");

	MQueue_Count = 0;
	CQueue_Count = 0;
	cur_cross = 0;
	sailing = 0;
	valid_boat = 0;
	boat_loaded = 0;
	p_count = 0;
	pass = new int[3];
	pass2 = new int[3];
	for( int i = 0; i < 3; ++i ) {
		pass[i] = -1;
		pass2[i] = -1;
	}
};

// ----------------------------------------------------------- 
// FUNCTION BoatMonitor()                        
//     BoatMonitor destructor
// PARAMETER USAGE :       
//		m: max number of crossings
// FUNCTION CALLED :                                           
//		n/a
// -----------------------------------------------------------
BoatMonitor::~BoatMonitor(){
	delete MQueue;
	delete CQueue;
	delete GetOff;
	delete[] pass;
};

// ----------------------------------------------------------- 
// FUNCTION CannibalArrives                        
//		Called by cannibals when they want to cross the river.
// PARAMETER USAGE :       
//		id: The id of the cannibal trying to cross.
// FUNCTION CALLED :     
//		Monitor::MonitorBegin()
//		Monitor::MonitorEnd()
//		Condition::Wait()
// -----------------------------------------------------------
void BoatMonitor::CannibalArrives(int id){
	MonitorBegin();
	++CQueue_Count;
	if( sailing == 0 && ( CQueue_Count >= 3 || MQueue_Count >= 2 ) ){
		sailing = 1;
		valid_boat = 1;
	}
	CQueue->Wait();
	pass[p_count] = id;
	pass2[p_count] = CANNIBAL;
	++p_count;
	if( p_count == 3 )
		boat_loaded = 1;
	GetOff->Wait();
	--p_count;
	if( p_count == 0 ){
		valid_boat = 0;
		boat_loaded = 0;
	}
	MonitorEnd();
};

// ----------------------------------------------------------- 
// FUNCTION MissionaryArrives                        
//		Called by missionaries when they want to cross the river.
// PARAMETER USAGE :       
//		id: The id of the missionary trying to cross.
// FUNCTION CALLED :     
//		Monitor::MonitorBegin()
//		Monitor::MonitorEnd()
//		Condition::Wait()
// -----------------------------------------------------------
void BoatMonitor::MissionaryArrives(int id){
	MonitorBegin();
	++MQueue_Count;
	if( sailing == 0 &&
		( MQueue_Count >= 3 || CQueue_Count > 0 ) ){
		sailing = 1;
		valid_boat = 1;
	}
	MQueue->Wait();
	pass[p_count] = id;
	pass2[p_count] = MISSIONARY;
	++p_count;
	if( p_count == 3 )
		boat_loaded = 1;
	GetOff->Wait();
	pass[p_count] = -1;
	pass2[p_count] = -1;
	--p_count;
	if( p_count == 0 ){
		valid_boat = 0;
		boat_loaded = 0;
	}
	MonitorEnd();
};

// ----------------------------------------------------------- 
// FUNCTION BoatReady                        
//		Called by the boat when it detects a valid boat has been
//		assembled.
// PARAMETER USAGE :       
//		n/a
// FUNCTION CALLED :     
//		Monitor::MonitorBegin()
//		Monitor::MonitorEnd()
//		Condition::Signal()
// -----------------------------------------------------------
void BoatMonitor::BoatReady(){
	MonitorBegin();
	char buf[200];
	if( MQueue_Count >= 3 ){
		for( int i = 0; i < 3; ++i ) MQueue->Signal();
		sprintf( buf, "MONITOR (%d): Three missionaries are selected (%d, %d, %d)\n",
				cur_cross, pass[0], pass[1], pass[2] ); 
	} else if ( CQueue_Count >= 3 ) {
		for( int i = 0; i < 3; ++i ) CQueue->Signal();
		sprintf( buf, "MONITOR (%d): Three cannibals are selected (%d, %d, %d)\n",
				cur_cross, pass[0], pass[1], pass[2] ); 
	} else {
		MQueue->Signal();
		MQueue->Signal();
		CQueue->Signal();
		int c, m1, m2;
		c = m1 = m2 = -1;
		if( pass[0] == CANNIBAL ){ c = 0; m1 = 1; m2 = 2; }
		else if ( pass[1] == CANNIBAL ){ c = 1; m1 = 0; m2 = 2; }
		else { c = 2; m1 = 0; m1 = 1; }
		sprintf( buf, "MONITOR (%d): Two missionaries (%d, %d) and one cannibal are selected (%d)\n",
				cur_cross, pass[m1], pass[m2], pass[c] ); 
	}
	my_printf( buf );
	MonitorEnd();
};

// ----------------------------------------------------------- 
// FUNCTION BoatDone                        
//		Called by the boat when it detects that all three 
//		passengers have boarded.
// PARAMETER USAGE :       
//		n/a
// FUNCTION CALLED :     
//		Monitor::MonitorBegin()
//		Monitor::MonitorEnd()
//		Condition::Signal()
// -----------------------------------------------------------
void BoatMonitor::BoatDone(){
	MonitorBegin();
	char buf[200];
	for( int i = 0; i < 3; ++i ) GetOff->Signal();
	sailing = 0;
	++cur_cross;
	if( cur_cross == max_crossings ){
		sprintf( buf, "MONITOR: %d crossings have been made.\nThe river is now closed indefinitely.\n",
			 max_crossings);
		my_printf( buf );
	}
	MonitorEnd();
};

// ----------------------------------------------------------- 
// FUNCTION getCrossings                      
//		Returns the current number of completed crossings.
// PARAMETER USAGE :       
//		n/a
// FUNCTION CALLED :     
//		n/a
// -----------------------------------------------------------
int BoatMonitor::getCrossings(){
	return cur_cross;
};

// ----------------------------------------------------------- 
// FUNCTION getCrossings                      
//		Helper function used by the boat to detect when a valid
//		load of passengers is waiting on the shore. Returns true
//		when any valid load of passengers is waiting (3m, 3c and 
//		2m/1c)
// PARAMETER USAGE :       
//		n/a
// FUNCTION CALLED :     
//		n/a
// -----------------------------------------------------------
int BoatMonitor::getValid(){
	return valid_boat;
};

// ----------------------------------------------------------- 
// FUNCTION getCrossings                      
//		Helper function used by the boat to retrieve the current
//		status of passengers boarding. Returns true when 3
//		passengers have boarded.
// PARAMETER USAGE :       
//		n/a
// FUNCTION CALLED :     
//		n/a
// -----------------------------------------------------------
int BoatMonitor::getLoaded(){
	return boat_loaded;
};


// ----------------------------------------------------------- 
// FUNCTION getCrossings                      
//		Helper function used by the boat to retrieve the passenger
//		list currently riding.
// PARAMETER USAGE :       
//		n/a
// FUNCTION CALLED :     
//		n/a
// -----------------------------------------------------------
char* BoatMonitor::getBoat(){
	char buf[100];
	char *ret = new char[100];
	for( int i = 0; i < 2; ++i ){
		if( pass2[i] == MISSIONARY ){
			sprintf( buf, "m%d, ", pass[i] );
			strcat( ret, buf );
		} else {
			sprintf( buf, "c%d, ", pass[i] );
			strcat( ret, buf );
		}
	}
	
	if( pass2[2] == MISSIONARY ){
		sprintf( buf, "m%d", pass[2] );
		strcat( ret, buf );
	} else {
		sprintf( buf, "c%d", pass[2] );
		strcat( ret, buf );
	}
	
	return ret;
};
#endif