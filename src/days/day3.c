#include "parser.h"

extern FILE *load()
{
    return fopen("inputs/day_3.txt", "r");
}

extern void part1(FILE *filePtr, char *result)
{
    char *source = NULL;
    Token *tokens = NULL;
    int sum = 0;

    source = readSource(filePtr);
    tokens = lexer(&source, false);
    Node *nodes = parser(&tokens, source);
    sum = interpreter(nodes);
    free(source);
    arrfree(tokens);
    arrfree(nodes);

    sprintf(result, "%d", sum);
}

extern void part2(FILE *filePtr, char *result)
{
    char *source = NULL;
    Token *tokens = NULL;
    int sum = 0;

    source = readSource(filePtr);
    tokens = lexer(&source, true);
    Node *nodes = parser(&tokens, source);
    sum = interpreter(nodes);
    free(source);
    arrfree(tokens);
    arrfree(nodes);

    sprintf(result, "%d", sum);
}
