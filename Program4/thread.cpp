// ----------------------------------------------------------- 
// NAME : Jake Herson                User ID: jaherson 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    A couple of lines describing your program briefly        
// -----------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "thread.h"
using namespace std;

/*thread for the momma eagle*/
void *motherEagleThread_func(void *motherEagleArgs){
	struct motherEagleData *args;
	args=(struct motherEagleData *) motherEagleArgs;
	
	safePrint("Mother eagle started.");
	
    for(int i=0;i<numFeedings;i++){
        pthread_sleep(MOTHEREAGLE_SLEEP_TIME);
        goto_sleep();
        pthread_sleep(MOTHEREAGLE_SLEEP_TIME);
        food_ready(i);
    }
	
	motherRetired=true;
	
	//If motherEagle is retiring then signal all children to wake up and fly away
	for(int i=0;i<numBabyEagles;i++){
		sem_post(&semWaiting);
	}
	
	
	return (void*)NULL;
}

/*thread for the baby eagle*/
void *babyEagleThread_func(void *babyEagleArgs){
	struct babyEagleData *args;
	args=(struct babyEagleData *) babyEagleArgs;
	
	safePrint(strSpaces(args->eagleId)+"Baby eagle "+itos(args->eagleId)+" started.");
	
    while(1){

        ready_to_eat(args->eagleId);
        pthread_sleep(BABYEAGLE_EAT_TIME);

        finish_eating(args->eagleId);
        pthread_sleep(BABYEAGLE_PLAY_TIME);
    }
	return (void*) NULL;
}