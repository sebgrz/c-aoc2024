#include "common.h"

enum TokenType
{
    None,
    LeftParen,
    RightParen,
    Comma,
    MulIns,
    Number,
};

typedef struct Token
{
    enum TokenType type;
    int start, length;
} Token;

char *readSource(FILE *fPtr);
int *parseMulToken(char **token);
void addToken(Token **tokens, Token token);
Token *lexer(char **source);

extern FILE *load()
{
    return fopen("inputs/day_3_sample.txt", "r");
}

extern void part1(FILE *filePtr, char *result)
{
    char *source = NULL;
    Token *tokens = NULL;
    int sum = 0;

    source = readSource(filePtr);
    tokens = lexer(&source);

    printf("\nREAD TOKENS - %ld\n", arrlen(tokens));
    for (int i = 0; i < arrlen(tokens); i++)
    {
        Token t = tokens[i];
        printf("token { .type %d, .start %d, length %d } \n", t.type, t.start, t.length);
    }

    free(source);
    arrfree(tokens);

    sprintf(result, "%d", sum);
}

void parser(Token **tokens, char *source)
{
    long size = arrlen(*tokens);
    long index = 0;
    for (; index < size;)
    {
        Token t = *(*tokens + index);
        switch (t.type)
        {
        case LeftParen:
        case RightParen:
        case MulIns:
        case Comma:
        case Number:
        default:
            continue;
        }
        index++;
    }
}

Token *lexer(char **source)
{
    Token *tokens = NULL;
    int index = 0;
    char ch;
    Token token = {};
    for (;;)
    {
        ch = *(*source + index);
        if (ch == '\0')
        {
            break;
        }

        if (ch >= 'a' && ch <= 'z')
        {
            token = (Token){
                .type = MulIns,
                .start = index,
                .length = 1,
            };
            ch = *(*source + index + 1);
            while ((ch >= 'a' && ch <= 'z'))
            {
                index++;
                token.length = index - token.start + 1;
                ch = *(*source + index + 1);
            }

            char *tokenCheck = malloc(token.length + 1);
            strncpy(tokenCheck, *source + token.start, token.length);
            *(tokenCheck + token.length) = '\0';
            char *tokenLastThreeCharsPtr = tokenCheck + token.length - 3;
            if (token.length >= 3 && strcmp(tokenLastThreeCharsPtr, "mul") == 0)
            {
                addToken(&tokens, token);
            }

            free(tokenCheck);
        }
        else if (ch >= '0' && ch <= '9')
        {
            token = (Token){.type = Number, .start = index, .length = 0};
            ch = *(*source + index + 1);
            while ((ch >= '0' && ch <= '9'))
            {
                index++;
                token.length = index - token.start + 1;
                ch = *(*source + index + 1);
            }
            addToken(&tokens, token);
        }
        else if (ch == '(')
        {
            token = (Token){
                .type = LeftParen,
                .start = index,
                .length = 1,
            };
            addToken(&tokens, token);
        }
        else if (ch == ')')
        {
            token = (Token){
                .type = RightParen,
                .start = index,
                .length = 1,
            };
            addToken(&tokens, token);
        }
        else if (ch == ',')
        {
            token = (Token){
                .type = Comma,
                .start = index,
                .length = 1,
            };
            addToken(&tokens, token);
        }
        else
        {
        }

        index++;
    }

    return tokens;
}

void addToken(Token **tokens, Token token)
{
    if (token.type == None)
    {
        return;
    }

    arrput(*tokens, token);
}

char *readSource(FILE *fPtr)
{
    fseek(fPtr, 0, SEEK_END);
    long size = ftell(fPtr);
    rewind(fPtr);

    char *source = malloc(size * sizeof(char));
    fread(source, size, sizeof(source), fPtr);
    return source;
}

extern void part2(FILE *filePtr, char *result)
{
    sprintf(result, "NOT IMPLEMENTED");
}
