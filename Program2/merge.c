// ----------------------------------------------------------- 
// NAME : Jake Herson                        
// DUE DATE : 10/14/16                            
// PROGRAM ASSIGNMENT #2                                        
// FILE NAME : merge.c           
// PROGRAM PURPOSE :                                           
// 		Merges two arrays to sort their values and returns
//		one sorted array.
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
 * Partition the data into two parts, elements smaller than the pivot to the
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
     
    if ((pid = fork()) < 0) {                /* fork a child process  */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    } else if (pid == 0) {          /* for the child process:         */
        if (execvp(*argv, argv) < 0) {        /* execute the command  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    } else {                                  /* for the parent:      */
        while (wait(&status) != pid){};       /* wait for completion  */  
    }
}

// ----------------------------------------------------------- 
// FUNCTION  merge                         
//     the function that sorts the numbers                           
// PARAMETER USAGE :                                           
//    array of numbers, the beginning, end, and halfway point               
// FUNCTION CALLED :                                           
//    n/a          
// ----------------------------------------------------------- 
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
// ----------------------------------------------------------- 
// FUNCTION  mergesort                         
//     the recursiveness of the mergesort algorithm                           
// PARAMETER USAGE :                                           
//    the array, the left side, the right side               
// FUNCTION CALLED :                                           
//    mergsort and merge          
// ----------------------------------------------------------- 
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
 
        merge(arr, l, m, r);
    }
}
 
// ----------------------------------------------------------- 
// FUNCTION SWAP                       
//     swaps the elements                           
// PARAMETER USAGE :                                           
//  	array to partition, left side, right side       
// FUNCTION CALLED :                                           
//    	partition       
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
//  	argv[] terminal vars      
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
		merge(ShmPTR->data,atoi(argv[1]),atoi(argv[2]));
	} else {
		printf("   ### Q-PROC(%d): entering with a[%d..%d]\n" , getpid(), atoi(argv[1]), atoi(argv[2]));
		mergeSort(ShmPTR->data,atoi(argv[1]),atoi(argv[2]));
	}   
    ShmPTR->status = TAKEN;
    // printf("   Client has informed server data have been taken...\n");
    shmdt((void *) ShmPTR);
    // printf("   Client has detached its shared memory...\n");
    // printf("   Client exits...\n");
    exit(0);
}