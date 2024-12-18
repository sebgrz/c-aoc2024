#include "common.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

size_t compareLevels(int *l1, int *l2);

extern FILE *load()
{
    return fopen("inputs/day_2.txt", "r");
}

extern void part1(FILE *filePtr, char *result)
{
    int sumSafe = 0;
    int lineNo = 0;
    char *line = NULL;
    size_t lineSize = 0;
    while (getline(&line, &lineSize, filePtr) > 0)
    {
        char *report = line;
        char *level = NULL;
        int prevLevel = -1;
        size_t levelsState = 0;
        for (;; report = NULL)
        {
            level = strtok(report, " ");
            if (level == NULL)
            {
                sumSafe++;
                break;
            }
            int iLevel = atoi(level);
            if (prevLevel < 0)
            {
                prevLevel = iLevel;
                continue;
            }

            size_t newLevelsState = compareLevels(&prevLevel, &iLevel);
            prevLevel = iLevel;
            if (newLevelsState == 0) // ERROR: break Any two adjacent levels differ by at least one and at most three.
            {
                break;
            }

            if (levelsState == 0) // initalize state
            {
                levelsState = newLevelsState;
                continue;
            }

            if (levelsState != newLevelsState) // ERROR: break The levels are either all increasing or all decreasing.
            {
                break;
            }
        }
    }
    free(line);
    sprintf(result, "%d", sumSafe);
}

size_t compareLevels(int *l1, int *l2)
{
    int diff = *l1 - *l2;
    if (diff == 0)
    {
        return 0;
    }

    int absDiff = abs(diff);
    if (absDiff >= 1 && absDiff <= 3)
    {
        return diff < 0 ? -1 : 1;
    }
    return 0;
}

extern void part2(FILE *filePtr, char *result)
{
    sprintf(result, "NOT IMPLEMENTED");
}
