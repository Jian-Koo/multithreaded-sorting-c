#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "SortingFunction.h"

/**
 * @brief Open the file and write its content (integers) into an array.
 *
 * @param pFileName a file name
 * @param pArray a pointer to an array
 * @param pArraySize a pointer to an integer, array size
 * 
 * @return TRUE or FALSE, 'boolean'
 */
int readFile(char* pFileName, int pArray[], int* pArraySize)
{
    FILE *file = fopen(pFileName, "r");
    if (!file) 
    {
        perror("Error opening file\n");
        return FALSE;
    }

    /* Read the opened file and write the integers into the array while incrementing the arraySize */
    while (fscanf(file, "%d", &pArray[*pArraySize]) != EOF && *pArraySize < MAX_SIZE_ARRAY) 
    {
        (*pArraySize)++;
    }
    fclose(file);
    return TRUE;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        perror("Usage : ./sss <filename>\n");
        return 1;
    }

    int array[MAX_SIZE_ARRAY], arraySize = 0;
    int swapsOfT1 = 0, swapsOfT2 = 0;
    int idOne = 1, idTwo = 2;
    int noSwapCount = 0, turn = 1;
    ThreadInfo* threadOneInfo = (ThreadInfo*)malloc(sizeof(ThreadInfo));
    ThreadInfo* threadTwoInfo = (ThreadInfo*)malloc(sizeof(ThreadInfo));
    pthread_t T1, T2;
    pthread_mutex_t lock;
    pthread_cond_t cond;

    pthread_mutex_init(&lock, NULL); 
    pthread_cond_init(&cond, NULL); 

    int successfulReading = readFile(argv[1], array, &arraySize);
    
    /* Initialises the mutex lock and the condition variable with default attributes */

    if (successfulReading == FALSE)
    {
        free(threadOneInfo);
        free(threadTwoInfo);
        return 1;
    }

    printf("Initial Array : \n");
    printArray(array, arraySize); /* Print the initial array before going through sorting process */

    if (!threadOneInfo || !threadTwoInfo)
    {
        perror ("Error : Failed to create ThreadInfo\n");
        free(threadOneInfo);
        free(threadTwoInfo);
        return 1;
    }

    /* Creates and initialises ThreadInfo structs that are to be used for T1 and T2 later */
    createThreadInfo(threadOneInfo, idOne, array, &arraySize, &swapsOfT1, &turn, &noSwapCount, &lock, &cond); 
    createThreadInfo(threadTwoInfo, idTwo, array, &arraySize, &swapsOfT2, &turn, &noSwapCount, &lock, &cond);

    /* Creates threads T1 and T2, each executing 'sort' with their respective ThreadInfo structs */
    if (pthread_create(&T1, NULL, &sort, (void*)threadOneInfo) != 0 || 
        pthread_create(&T2, NULL, &sort, (void*)threadTwoInfo) != 0)
    {
        perror("Error : Failed to create threads\n");
        free(threadOneInfo);
        free(threadTwoInfo);
        return 1;
    }
    /* Assign the actual thread id to threadID component in the thread info struct*/
    threadOneInfo-> threadID = T1; 
    threadTwoInfo-> threadID = T2;

    /* Wait for threads T1 and T2 to complete before continuing */
    pthread_join(T1, NULL);
    pthread_join(T2, NULL);

    /* Print out all the necessary info, swaps of T1 and T2, sorted array, and the total swaps */
    printf("Thread ID %lu : total number of swaps = %d\n", (unsigned long)threadOneInfo->threadID, swapsOfT1);
    printf("Thread ID %lu : total number of swaps = %d\n", (unsigned long)threadTwoInfo->threadID, swapsOfT2);
    printf("\n");
    printf("Sorted Array : \n");
    printArray(array, arraySize);
    printf("Total swaps : %d\n", swapsOfT1 + swapsOfT2);

    /* Destroys the mutex lock and the condition variable, releasing any resources 
    associated with them */
    pthread_mutex_destroy(&lock); 
    pthread_cond_destroy(&cond); 

    free(threadOneInfo);
    free(threadTwoInfo);
    threadOneInfo = NULL;
    threadTwoInfo = NULL;

    return 0;
}
