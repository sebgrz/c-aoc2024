#include "day_plugin.h"
#include <stdlib.h>

int main()
{
    FILE *file = load();
    char *result = malloc(0);

    part1(file, result);
    printf("result p1: %s\n", result);

    fseek(file, 0, SEEK_SET);

    part2(file, result);
    printf("result p2: %s\n", result);
    free(result);

    fclose(file);

    return 0;
}
