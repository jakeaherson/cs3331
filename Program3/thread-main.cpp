// -----------------------------------------------------------
// NAME : Jake Herson                     User ID: jaherson
// DUE DATE : 10/28/2016
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//    	The purpose is to create n threads in which call
//    	thread-main.cpp and computes the x[j] + x[j-2^(i-1)] 
//    	it also prints out results to stdout.
// -----------------------------------------------------------

#include <iostream>
#include <math.h>
#include <stdio.h>
#include "thread.h"
#define X 200
#define Y 200

extern int B [X][Y]; //global array 

int main(int argc, char *argv[])
{
	SumThread *sumthread;
    int *array;
    int arraySize,          // number of elements in the array to be sorted

    i;
	int z = 0;
    cout << "Concurrent Prefix Sum Computation " << endl; 
    scanf(" %d ",&arraySize);
    int indx  = 0;
    int counter = 2;
	cout << "Number of input Data = " << arraySize << endl; 
    array = new int[arraySize]; // create the array
	
    while(indx < arraySize) {
		int num;
	    scanf(" %d ",&num);
        array[indx] = num;
        indx++;
        counter++;
    }


    cout << "Input Array:" << endl;
    cout << "   "; 
    for (i = 0; i < arraySize; i++) {
		cout << array[i] << "   ";
	}
	
	cout << endl;
	int fill = 0;
	
    while(fill < arraySize ) {
		B[0][fill] = array[fill];
        fill++;
    }
	
    // start threading 
    int a = 1;
    int j = 0;
    int num = log (arraySize) / log(2);
    while(a <= num) {
		cout << "Run " << a << ":" << endl;
        while(j < arraySize) {
			sumthread = new SumThread(a, j, array);
			sumthread->Begin();
			sumthread->Join();
			j++;
		} 

		a++; 
		if( a <= num) {
			cout << "Result after run " << a-1 << ":" << endl;
			z = 0;
			cout << "   ";
			while(z < arraySize) {
				cout << B[a-1][z] << "    "; 
				z++;
			}
			cout << " " << endl;
		}
		j = 0; 
    }


	cout << "Result after Final Run  " << num << ":" << endl;
    z = 0;
    cout << "   ";
    while(z < arraySize) {
        cout << B[num][z] << "    "; 
        z++;
    }
	cout << " " << endl;
	Exit();
     
	return 0; 
}