#include "common.h"

char *readWordSearch(FILE *fPtr, int *width, int *height);

extern FILE *load()
{
    return fopen("inputs/day_4_sample.txt", "r");
}

extern void part1(FILE *filePtr, char *result)
{
    int sum = 0, width, height;
    char *wordSearch = readWordSearch(filePtr, &width, &height);

    printf("WORD SEARCH %d, %d\n", width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%c", wordSearch[y * width + x]);
        }
        printf("\n");
    }

    arrfree(wordSearch);
    sprintf(result, "%d", sum);
}

extern void part2(FILE *filePtr, char *result)
{
    int sum = 0;

    sprintf(result, "%d", sum);
}

char *readWordSearch(FILE *fPtr, int *width, int *height)
{
    int index = 0, line = 0;
    char *wordSearch = NULL;

    for (;;)
    {
        char ch = fgetc(fPtr);
        if (ch == EOF)
        {
            *height = line;
            break;
        }
        if (ch == '\n')
        {
            *width = index;
            index = 0;
            line++;
        }
        else
        {
            arrput(wordSearch, ch);
            index++;
        }
    }
    return wordSearch;
}
