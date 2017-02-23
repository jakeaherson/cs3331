// ----------------------------------------------------------- 
// NAME : Jake Herson
// DUE DATE : 10/14/16                                     
// PROGRAM ASSIGNMENT #2                                        
// FILE NAME : qsort.c           
// PROGRAM PURPOSE :                                           
// 		Divides array and calls quicksort on itself to sort an 
//    	array.      
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

struct Memory1 {
	int  status1;
	int  data1[5];
};

struct Memory2 {
	int  status2;
	int  data2[4];
};
void swap(int[], int, int);
/*
 * Parition the data into two parts, elements smaller than the pivot to the
 * 'left' and elements larger to the 'right'.
 */
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
     
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid){}     /* wait for completion  */
               ;
     }
}

// ----------------------------------------------------------- 
// FUNCTION PARTITION                        
//     partition the array                           
// PARAMETER USAGE :                                           
//  	array to partition, left side, right side, and the pindx        
// FUNCTION CALLED :                                           
//    	quicksort and quicksortutil         
// ----------------------------------------------------------- 
int partition(int array[], int left, int right, int pivot_index)
{
	int pivot_value = array[pivot_index];
	int store_index = left;
	int i;

	swap(array, pivot_index, right);
	for (i = left; i < right; i++)
		if (array[i] <= pivot_value) {
			swap(array, i, store_index);
			++store_index;
		}
	swap(array, store_index, right);
	return store_index;
}

// ----------------------------------------------------------- 
// FUNCTION  quicksort                          
//     the function that does the actual sorting                            
// PARAMETER USAGE :                                           
//   	array to be sorts, left side, right side               
// FUNCTION CALLED :                                           
//    partition, parse, execute          
// ----------------------------------------------------------- 
void quicksort(int array[], int left, int right)
{
	int pivot_index = left;
	int pivot_new_index;
     
	/*
	 * Use -1 to initialize because fork() uses 0 to identify a process as a
	 * child.
	 */
	int lchild = -1;
	int rchild = -1;

	if (right > left) {
		int status; /* For waitpid() only. */

		pivot_new_index = partition(array, left, right, pivot_index);
		//printf("%d",pivot_new_index);                
		/*
		 * Parallize by processing the left and right partion siultaneously.
		 * Start by spawning the 'left' child.
		 */
		lchild = fork();
		if (lchild < 0) {
			perror("fork");
			exit(1);
		}
		if (lchild == 0) {
			/* The 'left' child starts processing. */
			char line[1056];
			sprintf(line,"%s %d %d %d", "./qsort",left,pivot_new_index - 1,1);

			//printf("everything that is being passed %s",line);
			char *args[64];     
			parse(line, args);       /*   parse the line   */
			execute(args); 
			exit(0);
			wait(4);

		} else {
			/* The parent spawns the 'right' child. */
			rchild = fork();
			if (rchild < 0) {
				perror("fork");
				exit(1);
			}
			if (rchild == 0) {
				wait(2);

				/* The 'right' child starts processing. */
				//quicksort(array, pivot_new_index + 1, right);
				//exit(0);

				char line[1056];
				sprintf(line,"%s %d %d %d", "./qsort",pivot_new_index + 1,right,1);
				//printf("everything that is being passed right %s",line);

				char *args[64];     
				parse(line, args);       /*   parse the line               */
				execute(args); 
				exit(0);
			}
		}
		/* Parent waits for children to finish. */
		waitpid(lchild, &status, 0);
		waitpid(rchild, &status, 0);
	}
}

// ----------------------------------------------------------- 
// FUNCTION  quicksortUtil                          
//     	the purpose of this function                            
// PARAMETER USAGE :                                           
//    	array[] is an array of ints, left side, right side
// FUNCTION CALLED :                                           
//    	itself          
// ----------------------------------------------------------- 
void quicksortUtil(int array[], int left, int right)
{
	int pivot_index = left;
	int pivot_new_index;
	
	/*
	 * Use -1 to initialize because fork() uses 0 to identify a process as a
	 * child.
	 */
	int lchild = -1;
	int rchild = -1;
		//pivot_new_index = partition(array, left, right, pivot_index);

	if(left < right){
		pivot_new_index = partition(array, left, right, pivot_index);
		quicksortUtil(array, pivot_new_index + 1, right);
		quicksortUtil(array, left, pivot_new_index-1);
	} else {
		exit(0);
	}
}
// ----------------------------------------------------------- 
// FUNCTION SWAP                       
//     swaps the elements                           
// PARAMETER USAGE :                                           
//  array to partition, left side, right side       
// FUNCTION CALLED :                                           
//    partition       
// -----------------------------------------------------------

/*
 * The actual sorting, swap elements in the array that is being sorted.
 */
void swap(int array[], int left, int right)
{
	int temp;
	temp = array[left];
	array[left] = array[right];
	array[right] = temp;
}

// ----------------------------------------------------------- 
// FUNCTION MAIN                       
//     Forks processes and deals with a lot of args                           
// PARAMETER USAGE :                                           
//  argv[] terminal vars      
// FUNCTION CALLED :                                           
//        
// -----------------------------------------------------------

void  main(int argc, char *argv[])
{
    // printf("here is the args %s, %s, %d , %d", argv[0],argv[1],atoi(argv[2]),atoi(argv[3]));
    key_t          ShmKEY;
    int            ShmID;
    struct Memory  *ShmPTR;
     
    ShmKEY = ftok(".", 'x');
    ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);
    if (ShmID < 0) {
        printf("*** shmget error (client) ***\n");
        exit(1);
    }
	// printf("   Client has received a shared memory of four integers...\n");
     
    ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
    if ((int) ShmPTR == -1) {
        printf("*** shmat error (client) ***\n");
        exit(1);
    }
    //  printf("   Client has attached the shared memory...\n");
     
    while (ShmPTR->status != FILLED){};
    
	if (atoi(argv[3]) == 0 ){
		printf("   ### Q-PROC(%d): entering with a[%d..%d]\n" , getpid(), atoi(argv[1]), atoi(argv[2]));
		quicksort(ShmPTR->data,atoi(argv[1]),atoi(argv[2]));
	} else {
		printf("   ### Q-PROC(%d): entering with a[%d..%d]\n" , getpid(), atoi(argv[1]), atoi(argv[2]));
		quicksortUtil(ShmPTR->data,atoi(argv[1]),atoi(argv[2]));
	}   
    ShmPTR->status = TAKEN;
    // printf("   Client has informed server data have been taken...\n");
    shmdt((void *) ShmPTR);
    // printf("   Client has detached its shared memory...\n");
    // printf("   Client exits...\n");
    exit(0);
}