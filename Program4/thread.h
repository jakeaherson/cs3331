// ----------------------------------------------------------- 
// NAME : Jake Herson                User ID: jaherson 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//    A couple of lines describing your program briefly        
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <queue>
#include <string>
#include <time.h>
#include <semaphore.h>
#include <sstream>

#define MOTHEREAGLE_SLEEP_TIME 1
#define BABYEAGLE_PLAY_TIME 2
#define BABYEAGLE_EAT_TIME 2
#define DEFAULT "10"
#define ARGS_MAX 4

using namespace std;

extern int numFeedingPots;
extern int numBabyEagles;
extern int numFeedings;
extern int fullPots;
extern int numWaitingBabyEagles;

sem_t semFeedingPots; //value is equivalent to the number of feedingPots available
sem_t motherEagle;    //0 means motherEagle is sleeping, 1 means motherEagle is active
sem_t semWaiting;     // number of waiting baby eagles that are ready to eat
pthread_mutex_t mutFeedingPots=PTHREAD_MUTEX_INITIALIZER;   //locking the feedingPots
pthread_mutex_t tty_lk=PTHREAD_MUTEX_INITIALIZER;           //locking the tty

struct motherEagleData{
	int numFeedings;
};

struct babyEagleData{
	int eagleId;
	int numFeedings;
};

/************************/
/*	Globals		*/
/************************/
int numFeedingPots;
int numBabyEagles;
int numFeedings;
int fullPots;

int numWaitingBabyEagles=0;
bool alreadyWoken=false;
bool motherRetired=false;

/*is only called by the mother eagle when she wants to take a nap.*/
void goto_sleep();

/*is called when the mother eagle has finished adding food in all m feeding pots.*/
void food_ready(int nthServing);

/*blocks the caller, a baby eagle, if all feeding pots are empty. One and only one baby eagle who finds out all feeding pots being empty should wake up the mother eagle. This function returns only if there is a feeding pot with food available to this baby eagle.*/
void ready_to_eat(int eagleId);

/*should be called when a baby eagle finishes his meal.*/
void finish_eating(int eagleId);

/*given function*/
int pthread_sleep (int seconds);

/*locks and unlock tty*/
void safePrint(string output);

/*converts int to string*/
string itos(int i);

/*returns a string with num spaces specified in input*/
string strSpaces(int numSpaces);
#endif