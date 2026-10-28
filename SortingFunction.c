#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "SortingFunction.h"

/**
 * @brief Creates a thread info struct that holds all information needed 
 * by a thread.
 *
 * @param pID thread logical id
 * @param pArray an array pointer
 * @param pArraySize the actual size of the array
 * @param pThreadSwaps a pointer to an integer used to track the total number of swaps 
 *                   of a thread (not a shared variable)
 * @param pTurn a pointer to an integer used to track the turn of the threads
 * @param pNoSwapCount a pointer to an integer used to track the number 
 *                     of iterations that has no swaps.
 * @param pLock a pointer to the shared mutex lock
 * @param pCond a pointer to the shared condition variable
 * 
 * @return void / nothing
 */
void createThreadInfo(ThreadInfo* pInfo, int pID, int* pArray, 
                    int* pArraySize, int* pThreadSwaps, int* pTurn, int* pNoSwapCount, 
                    pthread_mutex_t* pLock, pthread_cond_t* pCond)
{
    pInfo -> id = pID;
    pInfo -> array = pArray;
    pInfo -> arraySize = pArraySize;
    pInfo -> threadSwaps = pThreadSwaps;
    pInfo -> turn = pTurn;
    pInfo -> noSwapCount = pNoSwapCount;
    pInfo -> lock = pLock;
    pInfo -> cond = pCond;
}

/**
 * @brief Sorts the shared array. 
 * 
 * @param pThreadInfo a generic struct that stores informations needed by the thread.
 * 
 * @return NULL
 */
void* sort(void* pThreadInfo)  
{
    ThreadInfo* info = (ThreadInfo*)pThreadInfo;
    int id = info -> id;
    int* array = info -> array; 
    int* arraySize = info -> arraySize;
    int* threadSwaps = info -> threadSwaps;
    int* turn = info -> turn;
    int* noSwapCount = info -> noSwapCount;
    pthread_mutex_t* lock = info -> lock;
    pthread_cond_t* cond = info -> cond;
    int localSwaps, i;

    /* 
    Determine if this thread should start from 0 or 1, if it is T1, 
    starts from 0 then, else, starts from 1. 
    */
    int startIndex = (id == 1) ? 0 : 1;

    while (TRUE)
    {
        pthread_mutex_lock(lock); /* This thread tries to acquire the mutex lock */

        while (*turn != id) /* Check if it is its turn */
            pthread_cond_wait(cond, lock); /* If it is not its turn, release the lock and sleep until signaled */

        if (*noSwapCount >= 2) /* Check if noSwapCount is 2 or above, so that this thread knows to terminate */
        {
            *turn = (id == 1) ? 2 : 1; /* Before it terminates, set to the turn to the other thread */
            pthread_cond_signal(cond); /* Signals the thread that is waiting for cond variable */
            pthread_mutex_unlock(lock); /* And releases the mutex lock */
            break; /* Exits the while loop and terminates */
        }

        localSwaps = 0; /* This is used to track of the number of swap in this iteration of this thread */
                        /* It is set to 0 in every iteration before sorting process starts. */

        for (i = startIndex; i < *arraySize - 1; i+=2)
        {
            if (array[i] > array[i+1])
            {
                swapIntegers(&array[i], &array[i+1]);
                (*threadSwaps)++; /* Ttrack the total number swaps this thread has done so far */
                localSwaps++; /* Track the number of swaps in this iteration only */
            }
        }
        
        *noSwapCount = (localSwaps == 0) ? *noSwapCount + 1 : 0; /* Increments by 1 if no swap occurs in this iteration */
        *turn = (id == 1) ? 2 : 1; /* Sets the turn to the other thread */
        pthread_cond_signal(cond); /* Signals the thread that is waiting for cond variable */
        pthread_mutex_unlock(lock); /* Releases the mutex lock */
    }
    return NULL;
}

/**
 * @brief Swaps two integers.
 *
 * @param pA First integer
 * @param pB Second integer
 * 
 * @return void / nothing
 */
void swapIntegers(int* pA, int* pB)
{
    int temp = *pA;
    *pA = *pB;
    *pB = temp;
}

/**
 * @brief Prints the given array of integers
 *
 * @param pArray A pointer to an array of integers
 * @param pArraySize The actual size of the array given
 * @return void / nothing
 */
void printArray(int* pArray, int pArraySize)
{
    int i;
    for (i = 0; i < pArraySize; i++)
    {
        printf("%d ", pArray[i]);
    }
    printf("\n\n");
}
