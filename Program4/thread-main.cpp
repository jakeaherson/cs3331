// ----------------------------------------------------------- 
// NAME : Jake Herson                User ID: jaherson 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    A couple of lines describing your program briefly        
// -----------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include "thread.h"
#include "thread.cpp"
#include "thread-support.cpp"

int main(int argc, char *argv[]){
		
    if (argc != ARGS_MAX) {
        cerr << "Invalid command line - usage: <numFeedingPots> <numBabyEagles> <numFeedings>" << endl;
        exit(-1);
    }
	
	for(int i=1; i<ARGS_MAX; i++){
		if((int)atoi(argv[i]) == 0)
			argv[i] = DEFAULT;
	}
	
	numFeedingPots=(int)atoi(argv[1]);
	numBabyEagles=(int)atoi(argv[2]);
	numFeedings=(int)atoi(argv[3]);
	fullPots=0;
	
    sem_init(&semFeedingPots, 0, numFeedingPots);
    sem_init(&motherEagle, 0, 0);
	sem_init(&semWaiting, 0, 0);
	
	pthread_t motherEagle_tid;
	pthread_t babyEagleArr_tid[numBabyEagles];
	
	struct motherEagleData motherEagleArgs={ numFeedings };
	struct babyEagleData babyEagleArgs[numBabyEagles];
	
	int rc;//return code

	rc = pthread_create(&motherEagle_tid, NULL, motherEagleThread_func, (void*) &motherEagleArgs);
	if ( rc == -1){
		perror("pthread_create");
		return -1;
	}

	for(int i=0; i<numBabyEagles; i++){
		babyEagleArgs[i].eagleId=i;
		babyEagleArgs[i].numFeedings=numFeedings;
		rc=pthread_create(&babyEagleArr_tid[i], NULL, &babyEagleThread_func, (void*) &babyEagleArgs[i]);
		if ( rc == -1){
			perror("pthread_create");
			return -1;
		}
	}

    //do joins
    void *status;
    for(int t=0; t<numBabyEagles; t++) {
		int rc = pthread_join(babyEagleArr_tid[t], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n ", rc);
			exit(-1);
		}
	}
  
	//void *status;
	pthread_join(motherEagle_tid, &status);

	cout<<"Mother eagle retires after serving "<<numFeedings<<" feedings.  Game ends!!!"<<endl;

    //clean up
    sem_destroy(&semFeedingPots); 
    sem_destroy(&motherEagle);
    sem_destroy(&semWaiting);
    pthread_mutex_destroy(&mutFeedingPots);
    pthread_mutex_destroy(&tty_lk);
	return 0;
}