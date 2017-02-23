// ----------------------------------------------------------- 
// NAME : Jake Herson                        
// DUE DATE : 10/14/16                            
// PROGRAM ASSIGNMENT #2                                        
// FILE NAME : main.c           
// PROGRAM PURPOSE :                                           
//    Forks and calls exec for qsort and merge.        
// ----------------------------------------------------------- 
#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>



#define  NOT_READY  -1
#define  FILLED     0
#define  TAKEN      1

struct Memory {
	int  status;
	int  data[4];
};

struct Memory1{
	int  status;
	int  data[4];
};

struct Memory2 {
	int  status;
	int  data[4];
};

/* ----------------------------------------------------------------- */
/* FUNCTION parse:                                                 */
/*   This receives commandline and parses it corectly in an array  */
/* ----------------------------------------------------------------- */
void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

/* ----------------------------------------------------------------- */
/* FUNCTION execute:                                                 */
/*    This function receives a commend line argument list with the   */
/* first one being a file name followed by its arguments.  Then,     */
/* this function forks a child process to execute the command using  */
/* system call execvp().                                             */
/* ----------------------------------------------------------------- */
void  execute(char **argv)
{
     pid_t  pid;
     int    status;
     
    if ((pid = fork()) < 0) {     /* fork a child process             */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    } else if (pid == 0) {          /* for the child process:         */
        if (execvp(*argv, argv) < 0) {     /* execute the command  	  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    } else {                                  /* for the parent:      */
        while (wait(&status) != pid){};       /* wait for completion  */
    }
}


/* ----------------------------------------------------------------- */
/* FUNCTION main:                                                 */
/*    Deals with the command line and file i/o                     */
/* ----------------------------------------------------------------- */
void  main(int  argc, char *argv[])
{
    int size;
    scanf("%d",&size);

    key_t          ShmKEY;
    int            ShmID;
    struct Memory  *ShmPTR;
    pid_t  pid;
    int    status;

    ShmKEY = ftok(".", 'x');
    printf("*** MAIN: shared memory key = %d\n",ShmKEY);

    ShmID = shmget(ShmKEY, sizeof(struct Memory), IPC_CREAT | 0666);
    if (ShmID < 0) {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }
    printf("*** MAIN: shared memory created\n");
     
    ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
    if ((int) ShmPTR == -1) {
        printf("*** shmat error (server) ***\n");
        exit(1);
    }
    printf("*** MAIN: shared memory attached and is ready to use \n");
          
    ShmPTR->status  = NOT_READY;
    
    int indx  = 0;
    int counter = 2;
    printf("Input array for qsort has %d elements: \n", size);

    while(indx < size) {
        int num;
	    scanf(" %d ",&num);
        ShmPTR->data[indx] = num;
            
		printf(" %d ",ShmPTR->data[indx]);
	    indx++;
        counter++;
    }
    printf("\n");

    ShmPTR->status = FILLED;

    printf("*** MAIN: about to spawn the process for qsort \n");
    pid = fork();

    if (pid < 0) {
        printf("*** fork error (server) ***\n");
        exit(1);
    }

    else if (pid == 0) {
		// printf("   ### Q-PROC(%d): entering with a[%d..%d]" , pid, 0, size-1);
		char line[1056]; 
		sprintf(line,"%s %d %d %d", "./qsort",0,size-1,0);
		// printf("%s",line);
		char *args[64];     
		parse(line, args);       /*   parse the line               */
		execute(args); 
		exit(0);
    } 
    wait(&status);
          
                 
    while (ShmPTR->status != TAKEN)
        sleep(1);
          
    printf("\n*** MAIN: sorted array by qsort:\n");
    int i = 0;
     
    while(i < size) {
		printf(" %d ",ShmPTR->data[i]);
		i++;  
    }
    printf("\n");

    shmdt((void *) ShmPTR);
    printf("\n*** MAIN: shared memory successfully detached\n");
    shmctl(ShmID, IPC_RMID, NULL);
    printf("*** MAIN: shared memory successfully removed\n");
    exit(0);
}