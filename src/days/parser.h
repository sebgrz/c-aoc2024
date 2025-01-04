#include "common.h"

#define EXPECT_TOKEN(tokens, tokenType, tokenRef)                                                                      \
    {                                                                                                                  \
        int expected = expectToken(tokens, tokenType, tokenRef);                                                       \
        if (expected < 0)                                                                                              \
        {                                                                                                              \
            Node *eofNode = malloc(sizeof(Node));                                                                      \
            eofNode->type = N_EOF;                                                                                     \
            return eofNode;                                                                                            \
        }                                                                                                              \
        if (expected == 0)                                                                                             \
        {                                                                                                              \
            return NULL;                                                                                               \
        }                                                                                                              \
    }

enum TokenType
{
    T_None,
    T_LeftParen,
    T_RightParen,
    T_Comma,
    T_MulIns,
    T_Ins,
    T_Number,
    T_Char,
    T_EOF,
};

typedef struct Token
{
    enum TokenType type;
    int start, length;
} Token;

enum NodeType
{
    N_MulFunction,
    N_ArgNumber,
    N_EOF,
};

typedef struct Node
{
    enum NodeType type;
    struct Node *n1, *n2;
    union {
        int value;
    };
} Node;

char *readSource(FILE *fPtr);
void addToken(Token **tokens, Token token);
Token *lexer(char **source);
Node *parser(Token **tokens, char *source);

int interpreter(Node *nodes)
{
    int sum = 0;

    for (int i = 0; i < arrlen(nodes); i++)
    {
        Node *node = &nodes[i];
        switch (node->type)
        {
        case N_MulFunction:
            sum += node->n1->value * node->n2->value;
            break;
        default:
            break;
        }
    }

    return sum;
}

int tokenIndex = 0;
Token *nextToken(Token *tokens)
{
    return (tokens + tokenIndex++);
}

int expectToken(Token *tokens, enum TokenType type, Token **tokenRef)
{
    Token *token = nextToken(tokens);
    if (token->type == T_EOF)
    {
        return -1;
    }
    if (token->type != type)
    {
        return 0;
    }

    *tokenRef = token;
    return 1;
}

Node *argNode(int value)
{
    Node *node = malloc(sizeof(Node));
    node->type = N_ArgNumber;
    node->value = value;

    return node;
}

Node *parseFunction(Token *tokens, char *source)
{
    Token *tokenRef = NULL;
    EXPECT_TOKEN(tokens, T_MulIns, &tokenRef)
    EXPECT_TOKEN(tokens, T_LeftParen, &tokenRef)

    EXPECT_TOKEN(tokens, T_Number, &tokenRef)
    char *number = malloc(tokenRef->length + 1);
    *(number + tokenRef->length) = '\0';
    strncpy(number, source + tokenRef->start, tokenRef->length);
    int a = atoi(number);
    free(number);

    EXPECT_TOKEN(tokens, T_Comma, &tokenRef)

    EXPECT_TOKEN(tokens, T_Number, &tokenRef)
    number = malloc(tokenRef->length + 1);
    *(number + tokenRef->length) = '\0';
    strncpy(number, source + tokenRef->start, tokenRef->length);
    int b = atoi(number);
    free(number);

    EXPECT_TOKEN(tokens, T_RightParen, &tokenRef)

    Node *node = malloc(sizeof(Node));
    node->type = N_MulFunction;
    node->n1 = argNode(a);
    node->n2 = argNode(b);
    return node;
}

Node *parser(Token **tokens, char *source)
{
    Node *nodes = NULL;
    for (;;)
    {
        Node *funNode = parseFunction(*tokens, source);
        if (funNode == NULL)
        {
            continue;
        }
        if (funNode->type == N_EOF)
        {
            free(funNode);
            break;
        }

        arrput(nodes, *funNode);
    }

    return nodes;
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
            token = (Token){
                .type = T_EOF,
                .start = index,
                .length = 0,
            };
            addToken(&tokens, token);
            break;
        }

        if (ch >= 'a' && ch <= 'z')
        {
            token = (Token){
                .type = T_MulIns,
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
            if (!(token.length >= 3 && strcmp(tokenLastThreeCharsPtr, "mul") == 0))
            {
                token.type = T_Ins;
            }

            addToken(&tokens, token);

            free(tokenCheck);
        }
        else if (ch >= '0' && ch <= '9')
        {
            token = (Token){.type = T_Number, .start = index, .length = 1};
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
                .type = T_LeftParen,
                .start = index,
                .length = 1,
            };
            addToken(&tokens, token);
        }
        else if (ch == ')')
        {
            token = (Token){
                .type = T_RightParen,
                .start = index,
                .length = 1,
            };
            addToken(&tokens, token);
        }
        else if (ch == ',')
        {
            token = (Token){
                .type = T_Comma,
                .start = index,
                .length = 1,
            };
            addToken(&tokens, token);
        }
        else
        {
            token = (Token){
                .type = T_Char,
                .start = index,
                .length = 1,
            };
            addToken(&tokens, token);
        }

        index++;
    }

    return tokens;
}

void addToken(Token **tokens, Token token)
{
    if (token.type == T_None)
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
