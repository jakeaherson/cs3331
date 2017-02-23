/*
 * Name: Jake Herson		User ID: jaherson
 * Due: 12/12/14
 * Program 5
 * thread.cpp
 * 	Contains the implementations of the classes  and functions
 * 		specified in thread.h 
 */

#ifndef JAHERSON_THREAD_CPP
#define JAHERSON_THREAD_CPP

#include "ThreadClass.h"
#include "thread.h"

#include <stdio.h>

/*
 * my_printf()
 * 	An alias to cout that uses a mutex to ensure threadsafe prints
 * 
 * args
 * 	s: The string to print.
 * 
 * functions called
 * 	Mutex::Lock()
 * 	Mutex::Unlock()
 */
void my_printf(char *s){
	Print_Lock->Lock();
		cout << s << endl;
	Print_Lock->Unlock();
};

/*
 * ThreadColReader::ThreadColReader()
 * 	Construct for column readers
 * 
 * args
 * 	name: Thread name
 * 	index: Column to read
 * 	id: threadID
 * 
 * functions called
 * 	n/a
 */
ThreadColReader::ThreadColReader( char* name, int index, int threadID ) : Thread(name), index(index), threadID(threadID){
	UserDefinedThreadId = threadID;
};

/*
 * ThreadRowReader::ThreadRowReader()
 * 	Constructor for Row Readers
 * 
 * args
 * 	name: Thread name
 * 	index: Row to read
 * 	id: threadID
 * 
 * functions called
 * 	n/a
 */
ThreadRowReader::ThreadRowReader( char* name, int index, int threadID ) : Thread(name), index(index), threadID(threadID){
	UserDefinedThreadId = threadID;
};


/*
 * ThreadProcessor::ThreadProcessor()
 * 	Constructor for processor threads
 * 
 * args
 * 	name: Thread name
 * 	i,j: Index in output the thread computes
 * 	id: threadID
 * 
 * functions called
 * 	n/a
 */
ThreadProcessor::ThreadProcessor( char* name, int i, int j, int threadID ) : Thread(name), i(i), j(j), threadID(threadID)
{
	UserDefinedThreadId = threadID;
	left = NULL;
	right = NULL;
	up = NULL;
	down = NULL;
	t_r = -1;
	t_c = -1;
	storage = 0;
	hasD=hasU=hasL=hasR=0;
};

/* The following are blank/default constructors for threads
 * Only called/used to create pointer arrays of respective threads */
ThreadRowReader::ThreadRowReader(){};
ThreadColReader::ThreadColReader(){};
ThreadProcessor::ThreadProcessor(){};


/*
 * ThreadProcessor::setLeft()
 * 	Assigns a channel to be the threads left neighbor.
 * 
 * args
 * 	c: The channel to assign
 * 
 * functions called
 * 	n/a
 */
void ThreadProcessor::setLeft(SynChannel *c){
	left = c;
};

/*
 * ThreadProcessor::setRight()
 * 	Assigns a channel to be the threads right neighbor.
 * 
 * args
 * 	c: The channel to assign
 * 
 * functions called
 * 	n/a
 */
void ThreadProcessor::setRight(SynChannel *c){
	right = c;
};

/*
 * ThreadProcessor::setUp()
 * 	Assigns a channel to be the threads top neighbor.
 * 
 * args
 * 	c: The channel to assign
 * 
 * functions called
 * 	n/a
 */
void ThreadProcessor::setUp(SynChannel *c){
	up = c;
};

/*
 * ThreadProcessor::setDown()
 * 	Assigns a channel to be the threads bottom neighbor.
 * 
 * args
 * 	c: The channel to assign
 * 
 * functions called
 * 	n/a
 */
void ThreadProcessor::setDown(SynChannel *c){
	down = c;
};
/*
char* ThreadProcessor::hasNeighbors(){
	char *buf = new char[200];
	sprintf(buf, "%d,%d\nhasL:%d\nhasR:%d\nhasU:%d\nhasD:%d\n\n",
		i,j,hasL,hasR,hasU,hasD);
	return buf;
}
*/

/* 
 * ThreadColReader::ThreadFunc()
 * 	Column reader threads driver.
 * 	Reads integers from the column specific by index in B
 * 	until it reaches the end, then sends EOD.
 * 
 * args
 * 	n/a
 * 
 * functions called
 * 	sprintf()
 * 	Channel::Send()
 * 	my_printf()
 * 	Exit()
 */
void ThreadColReader::ThreadFunc(){
	int temp;
	char buf[100];
	sprintf(buf, "Row thread r[%d] starting", index);
	my_printf(buf);
	for(int i = 0; i < B_Y; ++i){
		temp = dataA[i][index];
		p[0][index]->up->Send( &temp, sizeof(int) );
		sprintf(buf, "Row thread r[%d] sent %d A[%d][%d] to P[0][%d]", index, temp, i, index, index);
		my_printf(buf);
	} 
	int *EOD = new int(-1);
	p[0][index]->up->Send( EOD, sizeof(int) );
	sprintf(buf, "   Row thread r[%d] sent %d to P[0][%d]", index, temp, index);
	my_printf(buf);
	Exit();
};

/* 
 * ThreadRowReader::ThreadFunc()
 * 	Row reader threads driver.
 * 	Reads integers from the row specific by index in A
 * 	until it reaches the end, then sends EOD.
 * 
 * args
 * 	n/a
 * 
 * functions called
 * 	sprintf()
 * 	Channel::Send()
 * 	my_printf()
 * 	Exit()
 */
void ThreadRowReader::ThreadFunc(){
	int temp;
	char buf[100];
	sprintf(buf, "   Column thread c[%d] starting", index);
	my_printf(buf);
	for(int i = 0; i < A_X; ++i){
		temp = dataB[index][i];
		p[index][0]->left->Send( &temp, sizeof(int) );
		sprintf(buf, "   Column thread c[%d] sent %d B[%d][%d] to P[%d][0]", index, temp, i, index, index);
		my_printf(buf);
	} 
	int *EOD = new int(-1);
	p[index][0]->left->Send( EOD, sizeof(int) );
	sprintf(buf, "   Column thread c[%d] sent %d to P[%d][0]", index, temp, index);
	my_printf(buf);
	Exit();
};

/* 
 * ThreadProcessor::ThreadFunc()
 * 	Processor thread driver.
 * 	Reads data from its channels, up and left if it
 * 	if is in the first row/col resp. or its neighbors
 * 	right and down if it is not. Then multiplies them
 * 	and adds them to the local variable storage.
 * 	When EOD is read the value in storage is put into output
 * 	the EOD is passed on and the thread terminates.
 * 
 * args
 * 	n/a
 * 
 * functions called
 * 	sprintf()
 * 	Channel::Send()
 * 	Channel::Receive()
 * 	my_printf()
 * 	Exit()
 */
void ThreadProcessor::ThreadFunc(){
	char buf[100];
	sprintf(buf, "       Thread P[%d,%d] started", i,j);
	my_printf(buf);
	int in1,in2;
	in1 = in2 = -2;
	for( ;; ){
		if( hasU == 1 ){
			up->Receive( &in1, sizeof(int) );
		} else {
			p[i-1][j]->down->Receive( &in1, sizeof(int) );
		}
		if( hasL == 1 ){
			left->Receive( &in2, sizeof(int) );
		} else {
			p[i][j-1]->right->Receive( &in2, sizeof(int) );
		}
		sprintf(buf, "       Thread P[%d,%d] received %d from left and %d from top", i, j, in1, in2);
		my_printf(buf);
		
		if(hasR==1 && hasD==1) sprintf(buf, "       Thread P[%d,%d] sent %d right and %d down", i, j, in1, in2);
		else if(hasR==1 && hasD!=1) sprintf(buf, "       Thread P[%d,%d] sent %d right and no value down (no neighbor)", i, j, in1, in2);
		else if(hasR!=0 && hasD==1) sprintf(buf, "       Thread P[%d,%d] sent %d down and no value right (no neighbor)", i, j, in1, in2);
		my_printf(buf);
		if( in1 == -1 || in2 == -1 ){
			sprintf(buf, "       Thread P[%d,%d] received EOD, saved result %d and terminated", i, j, storage);
			my_printf(buf);
			
			if( hasR == 1 ){
				right->Send( &in1, sizeof(int) );
			}
			if( hasD == 1 ){
				down->Send( &in2, sizeof(int) );
			}
			
			output[i][j]=storage;
			Exit();			
		} else {
			if( hasR == 1 ){
				right->Send( &in2, sizeof(int) );
			}
			if( hasD == 1 ){
				down->Send( &in1, sizeof(int) );
			}
		}
		storage = storage+(in1*in2);
	}
};
#endif