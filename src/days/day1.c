#include "common.h"

extern FILE *load()
{
    return fopen("inputs/day_1.txt", "r");
}

extern void part1(FILE *filePtr, char *result)
{
    int *list1 = NULL;
    int *list2 = NULL;
    int lineNo = 0;
    int a, b;
    while (fscanf(filePtr, "%d\t%d", &a, &b) > 0)
    {
        arrput(list1, a);
        arrput(list2, b);
        lineNo++;
    }

    quickSort(list1, 0, lineNo - 1);
    quickSort(list2, 0, lineNo - 1);

    int sum = 0;
    for (size_t line = 0; line < lineNo; line++)
    {
        sum += abs(list1[line] - list2[line]);
    }

    arrfree(list1);
    arrfree(list2);
    sprintf(result, "%d", sum);
}

extern void part2(FILE *filePtr, char *result)
{
    int *list1 = NULL;
    int *list2 = NULL;
    int lineNo = 0;
    int a, b;
    while (fscanf(filePtr, "%d\t%d", &a, &b) > 0)
    {
        arrput(list1, a);
        arrput(list2, b);
        lineNo++;
    }

    int sum = 0;
    for (int i = 0; i < lineNo; i++)
    {
        int count = 0;
        for (int l = 0; l < lineNo; l++)
        {
            if (list1[i] == list2[l])
            {
                count++;
            }
        }
        sum += list1[i] * count;
    }

    arrfree(list1);
    arrfree(list2);
    sprintf(result, "%d", sum);
}
