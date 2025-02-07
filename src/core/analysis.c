#ifndef __ZOST_CORE_C__
#define __ZOST_CORE_C__

typedef enum TokenType {
    LEXEME_SYMBOL = 0,
    LEXEME_LITERAL = 1,
} TokenType;

typedef struct Token {
    TokenType type;
    char* start;
    char* end;
} Token;

#endif /** __ZOST_CORE_C__ */