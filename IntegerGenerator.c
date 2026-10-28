#include <stdio.h>
#include <stdlib.h>
#include <time.h>

# define TRUE 1

/**
 * @brief This program generates up to 200 numbers between 0 and 300 randomly.
 * Users are required to input an integer between 0 to 200, and the program 
 * generates that amount of numbers into the given file.
 * 
 * The value of number can be changed, it is not necessarily to be 300 maximum.
 * 
 * This is an OPTIONAL program.
 */
int main(int argc, char* argv[]) 
{
    int i;
    int intSize = -1;
    char input[40];

    if (argc != 2)
    {
        perror("Usage : ./IntegerGenerator <filename>");
        return 1;
    }

    FILE *file = fopen(argv[1], "w");

    if (file == NULL) 
    {
        perror("Unable to open file");
        return 1;
    }

    srand(time(NULL));

    while (TRUE)
    {
        printf("Amount of integer (between 0 to 200) >> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Error reading input.\n");
            continue;
        }

        if (sscanf(input, "%d", &intSize) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (intSize >= 0 && intSize <= 200)
            break;

        printf("It must be between 0 and 200!\n");
    }

    for (i = 0; i < intSize; i++) 
    {
        fprintf(file, "%d", rand() % 300); /* generates a number between 0 and 300 */ 
        if (i != intSize - 1) 
        {
            fprintf(file, " ");  /* writes the generated number followed by a space to the file */
        }
    }
    fclose(file); 
    printf("%d integers have been written to '%s'.\n", intSize, argv[1]);

    return 0;
}
