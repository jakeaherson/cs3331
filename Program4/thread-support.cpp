// ----------------------------------------------------------- 
// NAME : Jake Herson                User ID: jaherson 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread-support.cpp            
// PROGRAM PURPOSE :                                           
//    A couple of lines describing your program briefly        
// -----------------------------------------------------------

#include "thread.h"
using namespace std;

/*is only called by the mother eagle when she wants to take a nap.*/
void goto_sleep(){
    safePrint("Mother eagle takes a nap");
	sem_wait(&motherEagle);
}

/*is called when the mother eagle has finished adding food in all m feeding pots.*/
void food_ready(int nthServing){

    pthread_mutex_lock(&mutFeedingPots);

	alreadyWoken=false;
	fullPots=numFeedingPots;
	cout<<"Mother eagle says \"Feeding ("<<nthServing + 1<<")\""<<endl;
	pthread_mutex_unlock(&mutFeedingPots);
	
	//wake up waiting baby eagles
	int semVal;
	sem_getvalue(&semFeedingPots, &semVal);

	for(int i=0;i<numFeedingPots;i++){
		numWaitingBabyEagles--;

		sem_post(&semWaiting);
	}
	sem_getvalue(&semFeedingPots, &semVal);	
}

/*blocks the caller, a baby eagle, if all feeding pots are empty. One and only one baby eagle who finds out all feeding pots being empty should wake up the mother eagle. This function returns only if there is a feeding pot with food available to this baby eagle.*/
void ready_to_eat(int eagleId){
	safePrint(strSpaces(eagleId)+"Baby eagle "+itos(eagleId)+" ready to eat, fullPots= "+itos(fullPots));
	
	bool fed=false;
	while(fed==false){

		pthread_mutex_lock(&mutFeedingPots);
		
		if(fullPots>0){
			sem_wait(&semFeedingPots);
			cout<<strSpaces(eagleId)<<"Baby eagle "<<eagleId<<" is eating using feeding pot "<< fullPots <<endl;
			cout.flush();
			fullPots--;	
			fed=true;
			cout.flush();
			pthread_mutex_unlock(&mutFeedingPots);
			sem_post(&semFeedingPots);
		}else if(motherRetired==false && fullPots ==0){
			if( alreadyWoken==false ){
				sem_post(&motherEagle);
				alreadyWoken=true;
				pthread_mutex_lock(&tty_lk);
				cout<<"Mother eagle is awoke by baby eagle "<<eagleId<<" and starts preparing food."<<endl;
				pthread_mutex_unlock(&tty_lk);
			}
			numWaitingBabyEagles++;
			pthread_mutex_unlock(&mutFeedingPots);

			sem_wait(&semWaiting);
			
		}else if(motherRetired==true && fullPots ==0){
			pthread_mutex_unlock(&mutFeedingPots);
			pthread_exit(NULL);
		}
	}
}

/*should be called when a baby eagle finishes his meal.*/
void finish_eating(int eagleId){
	safePrint(strSpaces(eagleId)+"Baby eagle "+itos(eagleId)+" finished eating");
	if(motherRetired==true && fullPots==0){
		pthread_exit(NULL);
	}
}

/******************************************************************************
pthread_sleep takes an integer number of seconds to pause the current thread We
provide this function because one does not exist in the standard pthreads library. We simply use a function that has a timeout.
*****************************************************************************/
int pthread_sleep (int seconds)
{
	pthread_mutex_t mutex;
	pthread_cond_t conditionvar;
	struct timespec timetoexpire;
	if(pthread_mutex_init(&mutex,NULL))
	{
		return -1;
	}
	if(pthread_cond_init(&conditionvar,NULL))
	{
		return -1;
	}
	//When to expire is an absolute time, so get the current time and add
	//it to our delay time
	timetoexpire.tv_sec = (unsigned int)time(NULL) + seconds;
	timetoexpire.tv_nsec = 100;
	return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
}

void safePrint(string output){
	pthread_mutex_lock(&tty_lk);
	cout<<output<<endl;
	cout.flush();
	pthread_mutex_unlock(&tty_lk);
}

string itos(int i){
	std::string s;
	std::stringstream out;
	out << i;
	return out.str();
}

/*returns a string with num spaces specified in input*/
string strSpaces(int numSpaces){
	string rs; //return string
	for(int i=0; i<numSpaces; i++){
		rs=rs+" ";
	}
	return rs;
}