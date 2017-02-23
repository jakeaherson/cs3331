// -----------------------------------------------------------
// NAME : Jake Herson                     User ID: jaherson
// DUE DATE : 10/28/2016
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//    	This program uses multithreaded programming to     
//    	implement the quicksort algorithm
// -----------------------------------------------------------

#ifndef THREAD_H_
#define THREAD_H_

// header file

#include "ThreadClass.h"

#define X 200
#define Y 200

//---------------------------------------------------------------//
//  Process Thread Class: 
//     each thread applies quicksort algorithm to divide the given
//     array and sorts them recursively
//---------------------------------------------------------------//

class SumThread : public Thread
{
     public:
          // constructor
          SumThread(int Lowerbound, int Upperbound, int Array[]); 

     private:
          void ThreadFunc();  // thread body
          int  lowerbound;    // lower bound of the sub-array to be sorted
          int  upperbound;    // upper bound of the sub-array to be sorted
          int  *a;            // pointing to array to be sorted
};

#endif /* THREAD_H_ */
