#ifndef SORTING_FUNCTION_H
#define SORTING_FUNCTION_H

#define MAX_SIZE_ARRAY 200
#define TRUE 1
#define FALSE 0

typedef struct ThreadInfo
{
    int id; /* Track the thread using id, this is only for the alternating purpose in the sorting function */
    pthread_t threadID; /* The thread's real id */
    int* array; /* The array that needs to be sorted */
    int* arraySize; /* The actual size of the array */
    int* threadSwaps; /* The total number of swaps by a thread */
    int* turn; /* Track the turn */
    int* noSwapCount; /* Track the number of consecutive no swap iterations */
    pthread_mutex_t* lock; /* A mutex lock shared by all threads */
    pthread_cond_t* cond; /* A condition shared by all threads */
} ThreadInfo;

void createThreadInfo(ThreadInfo* pInfo, int pID, int* pArray, 
                    int* pArraySize, int* pThreadSwaps, int* pTurn, int* pNoSwapCount, 
                    pthread_mutex_t* pLock, pthread_cond_t* pCond);
void* sort(void* pThreadInfo);
void swapIntegers(int* pNumOne, int* pNumTwo);
void printArray(int* pArray, int pArraySize);

#endif
