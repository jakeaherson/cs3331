/*
 * Name: Jake Herson		User ID: jaherson
 * Due: 12/12/14
 * Program 6
 * thread-main.cpp
 * 	Contains the driver for the program.
 * 		Creates all threads and assigns channels
 * 		to them. Then waits for all the threads to
 * 		terminate before printing the computed
 * 		matrix
 */

#ifndef JAHERSON_THREAD_MAIN_CPP
#define JAHERSON_THREAD_MAIN_CPP

#include "ThreadClass.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ThreadProcessor ***p;
int **output;
int **dataA;
int **dataB;
int A_X,A_Y,B_X,B_Y;
Mutex *Print_Lock;

int main( int argc, char **argv ){
	char line[30];
	char buf[100];
	int l,m,u,v;
	
	Print_Lock = new Mutex("print");
	
	/* start getting the first matrix */
	cin.getline(line, 100);
	l = atoi( strtok( line, " \n" ) );
	m = atoi( strtok( NULL, " \n" ) );
	char *str;
	str = new char[50];
	dataA = new int*[l];
	for(int i = 0; i < l; ++i){
		dataA[i] = new int[m];
	}
	for( int i = 0; i < l; ++i ){
		/* grab the row ... */
		cin.getline(line, 100);	
		for( int j = 0; j < m; ++j ){
			/* write it into A */
			if( j == 0 ) {
				str = strtok( line, " \n" );
				dataA[i][j] = atoi( str );
			} else {
				str = strtok( NULL, " \n" );
				dataA[i][j] = atoi( str );
			}
		}
	}
	
	/* start getting the first matrix */
	strcpy(str, "");
	cin.getline(line, 100);
	u = atoi( strtok( line, " \n" ) );
	v = atoi( strtok( NULL, " \n" ) );
	dataB = new int*[u];
	for(int i = 0; i < u; ++i){
		dataB[i] = new int[v];
	}
	for( int i = 0; i < u; ++i ){
		/* grab the row ... */
		cin.getline(line, 100);	
		for( int j = 0; j < v; ++j ){
			/* write it into B */
			if( j == 0 ) {
				str = strtok( line, " \n" );
				dataB[i][j] = atoi( str );
			} else {
				str = strtok( NULL, " \n" );
				dataB[i][j] = atoi( str );
			}
		}
	}
	A_X = l;
	A_Y = m;
	B_X = u;
	B_Y = v;
	/* print the input matrices out and create the output array */
	my_printf("*** from main ***");
	char out[200];
	char tmp[25];
	sprintf(out, "Matrix A: %d rows, %d columns\n", m, l);
	for(int i = 0; i < l; ++i){
		for(int j = 0; j < m; ++j){
			sprintf(tmp, "%d ", dataA[i][j]);
			strcat(out, tmp);
		} strcat(out, "\n");
	}
	my_printf(out);	
	
	sprintf(out, "Matrix B: %d rows, %d columns\n", u, v);
	for(int i = 0; i < u; ++i){
		for(int j = 0; j < v; ++j){
			sprintf(tmp, "%d ", dataB[i][j]);
			strcat(out, tmp);
		} strcat(out, "\n");
	}
	my_printf(out);
	
	output = new int*[A_X];
	for(int i = 0; i < A_X; ++i) output[i] = new int[B_Y];	
	/* array initialization completed */
	
	/* create the array to hold all the processors */
	p = new ThreadProcessor**[B_X];
	for(int i = 0; i < B_X; ++i)
		p[i] = new ThreadProcessor*[A_Y];
	
	int id = 0;
	for(int i = 0; i < B_X; ++i){
		for(int j = 0; j < A_Y; ++j){
			sprintf(buf, "processor(%d) -- %d,%d", id, i, j);
			p[i][j] = new ThreadProcessor(buf, i, j, id);
			if( i == 0 ) p[i][j]->hasU=1;
			if( j == 0 ) p[i][j]->hasL=1;
			if( j != A_Y-1 ) p[i][j]->hasR=1;
			if( i != B_X-1 ) p[i][j]->hasD=1;
			++id;
		}
	}
	
	/* processor array created */
	
	/* create the array for the row readers */
	ThreadRowReader *rr[A_Y];
	for(int i = 0; i < A_Y; ++i){
		sprintf(buf, "rowreader(%d) -- row: %d", id, i);
		rr[i] = new ThreadRowReader(buf, i, id);
		++id;
	}
	/* row reader array created */
	
	/* create the array for column readers */
	ThreadColReader *cr[B_X];
	for(int i = 0; i < B_X; ++i){
		sprintf(buf, "colreader(%d) -- col: %d", id, i);
 		cr[i] = new ThreadColReader(buf, i, id);
		++id;
	}
	/* column reader array created */
	
	/* assign the channels for right and down */
	for(int i = 0; i < B_X; ++i){
		for(int j = 0; j < A_Y; ++j){
			if( p[i][j]->hasR == 1 ){
				sprintf(buf, "%d to %d", p[i][j]->threadID, p[i][j+1]->threadID);
				p[i][j]->setRight( new SynChannel(buf, p[i][j]->threadID, p[i][j+1]->threadID) );
			}
			if( p[i][j]->hasD == 1 ){
				sprintf(buf, "%d to %d", p[i][j]->threadID, p[i+1][j]->threadID);
				p[i][j]->setDown( new SynChannel(buf, p[i][j]->threadID, p[i+1][j]->threadID) );
			}
		}
	}
	/* right and down channels created */
	
	/* create the channels for left */
	for(int i = 0; i < B_X; ++i){
		if( p[i][0]->hasL == 1 ){
			sprintf(buf, "%d to %d", p[i][0]->threadID, rr[i]->threadID);
			p[i][0]->setLeft( new SynChannel(buf, rr[i]->threadID, p[i][0]->threadID ) );
		}
	}
	/* left channels created */
	
	/* create the channels for up */
	for(int i = 0; i < A_Y; ++i){
		if( p[0][i]->hasU == 1 ){
			sprintf(buf, "%d to %d", p[0][i]->threadID, cr[i]->threadID);
			p[0][i]->setUp( new SynChannel(buf, cr[i]->threadID, p[0][i]->threadID ) );
		}
	}
	/* up channels created */
	
	/* start the threads */
	for(int i = 0; i < A_Y; ++i){
		rr[i]->Begin();
	}
	
	for(int i = 0; i < B_X; ++i){
		cr[i]->Begin();
	}
	
	for(int i = 0; i < B_X; ++i ){
		for(int j = 0; j < A_Y; ++j){
			p[i][j]->Begin();
		}
	}
	/* threads started */
	cout << "test" << endl;
	/* wait for threads to terminate */
	for(int i = 0; i < B_X; ++i){
		for(int j = 0; j < A_Y; ++j){
			p[i][j]->Join();
		}
	}
	for(int i = 0; i < B_X; ++i){
		rr[i]->Join();
	}
	
	for(int i = 0; i < A_Y; ++i){
		cr[i]->Join();
	}
	/* threads terminated */
	
	/* print out the final matrix */
	sprintf(out, "Matrix C: %d rows, %d columns\n", B_X, A_Y);
	for(int i = 0; i < B_X; ++i){
		for(int j = 0; j < A_Y; ++j){
			sprintf(tmp, "%d ", output[i][j]);
			strcat(out, tmp);
		} strcat(out, "\n");
	}
	my_printf(out);
	/* done printing */
	exit(0);
}

#endif