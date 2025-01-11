#include "common.h"
#include <stdbool.h>

char *readWordSearch(FILE *fPtr, int *width, int *height);
int searchPerLine(char *wordSearch, int line, int width, int height, bool xShapeSearch);

extern FILE *load()
{
    return fopen("inputs/day_4.txt", "r");
}

extern void part1(FILE *filePtr, char *result)
{
    int sum = 0, width, height;
    char *wordSearch = readWordSearch(filePtr, &width, &height);

    for (int y = 0; y < height; y++)
    {
        sum += searchPerLine(wordSearch, y, width, height, false);
    }

    arrfree(wordSearch);
    sprintf(result, "%d", sum);
}

extern void part2(FILE *filePtr, char *result)
{
    int sum = 0, width, height;
    char *wordSearch = readWordSearch(filePtr, &width, &height);

    // ignore first and last line - because we looking for A letter, the center of MAS word
    for (int y = 1; y < height - 1; y++)
    {
        sum += searchPerLine(wordSearch, y, width, height, true);
    }

    arrfree(wordSearch);
    sprintf(result, "%d", sum);
}

enum Direction
{
    D_UP,
    D_UPRIGHT,
    D_RIGHT,
    D_DOWNRIGHT,
    D_DOWN,
    D_DOWNLEFT,
    D_LEFT,
    D_UPLEFT
};

bool searchDirection(char *wordSearch, int line, int pos, int width, int height, enum Direction direction)
{
    char word[4];
    switch (direction)
    {
    case D_UP:
        if (line < 3)
        {
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            word[i] = wordSearch[(line - i) * width + pos];
        }
        break;
    case D_UPRIGHT:
        if (line < 3 || width - pos < 4)
        {
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            word[i] = wordSearch[(line - i) * width + pos + i];
        }
        break;
    case D_RIGHT:
        if (width - pos < 4)
        {
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            word[i] = wordSearch[line * width + pos + i];
        }
        break;
    case D_DOWNRIGHT:
        if (height - line < 4 || width - pos < 4)
        {
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            word[i] = wordSearch[(line + i) * width + pos + i];
        }
        break;
    case D_DOWN:
        if (height - line < 4)
        {
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            word[i] = wordSearch[(line + i) * width + pos];
        }
        break;
    case D_DOWNLEFT:
        if (height - line < 4 || pos < 3)
        {
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            word[i] = wordSearch[(line + i) * width + pos - i];
        }
        break;
    case D_LEFT:
        if (pos < 3)
        {
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            word[i] = wordSearch[line * width + pos - i];
        }
        break;
    case D_UPLEFT:
        if (line < 3 || pos < 3)
        {
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            word[i] = wordSearch[(line - i) * width + pos - i];
        }
        break;
    }

    return strcmp(word, "XMAS") == 0;
}

bool hasDiagonalMasWord(char *wordSearch, int width, int line, int pos, bool checkLeft)
{
    if (checkLeft)
    {
        return (wordSearch[(line - 1) * width + pos - 1] == 'M' && wordSearch[(line + 1) * width + pos + 1] == 'S') ||
               (wordSearch[(line - 1) * width + pos - 1] == 'S' && wordSearch[(line + 1) * width + pos + 1] == 'M');
    }
    else
    {
        return (wordSearch[(line - 1) * width + pos + 1] == 'M' && wordSearch[(line + 1) * width + pos - 1] == 'S') ||
               (wordSearch[(line - 1) * width + pos + 1] == 'S' && wordSearch[(line + 1) * width + pos - 1] == 'M');
    }
}

int searchPerLine(char *wordSearch, int line, int width, int height, bool xShapeSearch)
{
    int found = 0;

    for (int x = 0; x < width; x++)
    {
        if (wordSearch[line * width + x] == 'X' && !xShapeSearch)
        {
            for (int direction = D_UP; direction <= D_UPLEFT; direction++)
            {
                if (searchDirection(wordSearch, line, x, width, height, direction))
                {
                    found++;
                }
            }
        }
        else if (wordSearch[line * width + x] == 'A' && xShapeSearch)
        {
            // M S
            //  A
            // M S
            // ignore first and last column
            if (x == 0 || x == width - 1)
            {
                continue;
            }

            if (hasDiagonalMasWord(wordSearch, width, line, x, true) &&
                hasDiagonalMasWord(wordSearch, width, line, x, false))
            {
                found++;
            }
        }
    }

    return found;
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
