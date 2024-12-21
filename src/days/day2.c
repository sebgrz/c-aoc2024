#include "common.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool checkValidity(int *levels, size_t arrSize);
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

extern void part2(FILE *filePtr, char *result)
{
    int sumSafe = 0;
    int lineNo = 0;
    char *line = NULL;
    size_t lineSize = 0;
    while (getline(&line, &lineSize, filePtr) > 0)
    {
        char *report = line;
        char *level = NULL;
        int *levels = NULL;
        for (;; report = NULL)
        {
            level = strtok(report, " ");
            if (level == NULL)
            {
                break;
            }
            arrpush(levels, atoi(level));
        }

        bool valid = checkValidity(levels, arrlen(levels));
        if (valid)
        {
            sumSafe++;
        }
        else
        {
            int levelsCount = arrlen(levels);
            int *levelsWithoutOne = malloc(sizeof(int) * levelsCount - 1);

            for (int i = 0; i < levelsCount; i++)
            {
                // fill new arr
                int levelsWithoutOneCounter = 0;
                for (int levelNo = 0; levelNo < levelsCount; levelNo++)
                {
                    if (i != levelNo)
                    {
                        levelsWithoutOne[levelsWithoutOneCounter] = levels[levelNo];
                        levelsWithoutOneCounter++;
                    }
                }

                if (checkValidity(levelsWithoutOne, levelsCount - 1))
                {
                    sumSafe++;
                    break;
                }
            }

            free(levelsWithoutOne);
        }

        arrfree(levels);
    }
    free(line);
    sprintf(result, "%d", sumSafe);
}

bool checkValidity(int *levels, size_t arrSize)
{
    int prevLevel = -1;
    size_t levelsState = 0;
    bool isValid = true;
    for (int i = 0; i < arrSize; i++)
    {
        int iLevel = levels[i];
        if (prevLevel < 0)
        {
            prevLevel = iLevel;
            continue;
        }

        size_t newLevelsState = compareLevels(&prevLevel, &iLevel);
        prevLevel = iLevel;
        if (newLevelsState == 0) // ERROR: break Any two adjacent levels differ by at least one and at most three.
        {
            isValid = false;
            break;
        }

        if (levelsState == 0) // initalize state
        {
            levelsState = newLevelsState;
            continue;
        }

        if (levelsState != newLevelsState) // ERROR: break The levels are either all increasing or all decreasing.
        {
            isValid = false;
            break;
        }
    }
    return isValid;
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
