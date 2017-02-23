// -----------------------------------------------------------
// NAME : Jake Herson                     User ID: jaherson
// DUE DATE : 10/28/2016
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//    	The purpose is to create threads for thread-main and
//    	compute the gap.
// -----------------------------------------------------------

#include <iostream>
#include <math.h>
#include "thread.h"

#define X 200
#define Y 200

int B [X][Y];
//---------------------------------------------------------------//
//  QuickSort Class: 
//     	constructor
//---------------------------------------------------------------//

SumThread::SumThread(int Lowerbound, int Upperbound, int Array[]) 
                :lowerbound(Lowerbound), upperbound(Upperbound), a(Array)
{
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Sorting" << '(' << Lowerbound 
			   << ':' << Upperbound << ')'
               << '\0';
	cout << "     Thread " << upperbound << " Created " << endl;
}

// ----------------------------------------------------------- 
// FUNCTION  SumThread                       
// 		Purpose is to compute x[j] + x[j-2^(i-1)] and store
// 		in global array B.                             
// PARAMETER USAGE :                                           
//    	a list of all parameters and their meaning               
// FUNCTION CALLED :                                           
//    	a list of functions that are called by this one          
// ----------------------------------------------------------- 

void SumThread::ThreadFunc()
{
	Thread::ThreadFunc();

	if(upperbound-pow(2,lowerbound-1) < 0 ) {
		B[lowerbound][upperbound] = B[lowerbound-1][upperbound];
		cout << "     Thread " << upperbound << " computes x[" 
			 << upperbound << "]" << " + x[" << upperbound-(int)pow(2,(lowerbound-1)) 
			 << "]" << endl;
	} else {
		int part1 = B[lowerbound-1][upperbound];
		// cout <<  part1 << "Part 1 " << endl;
		int part2 =  B[lowerbound-1][upperbound-(int)pow(2,lowerbound-1)];
		//  cout <<  part2 << "part 2 " <<  endl;
		int part3= part1+part2;
		// cout <<  part3 << "part 3 " <<  endl;
		B[lowerbound][upperbound] = part3;
		cout << "     Thread " << upperbound << " computes x[" 
		     << upperbound << "]" << " + x[" << upperbound-(int)pow(2,(lowerbound-1)) 
			 << "]" << endl;
	}
	Exit();      // done and exit
}

// end of thread.cpp