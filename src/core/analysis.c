#ifndef __ZOST_CORE_ANALYSIS_C__
#define __ZOST_CORE_ANALYSIS_C__

#include <stdio.h>

#include "../../vendor/ckastal/include/prefixed_length_string.c"
#include "../../vendor/ckastal/include/syntax_utils.h"

enum_str_setup(
    LexemeKind,
    "LEX_KIND",
    LEX_KIND_STRINGS,
    lexeme_kind_to_str,
    /** This will start as set to zero in the enum */
    LEX_KIND_INCOMPLETE,
    /** Other error or aside of language lexeme types */
    (LEX_KIND_ERROR)(LEX_KIND_UNKNOWN)(LEX_KIND_EOF)

    /* Comments */
    (LEX_KIND_COMMENT_LINE_START)(LEX_KIND_COMMENT_BLOCK_START)
    /** Arithmetical operators */
    (LEX_KIND_ARITHMETIC_OPERATOR)
    /** Logical operators */
    (LEX_KIND_EQUALS)(LEX_KIND_GT)(LEX_KIND_GTE)(LEX_KIND_LT)(LEX_KIND_LTE)(LEX_KIND_OR)(LEX_KIND_AND)
    /** Delimiters  */
    (LEX_KIND_NEW_LINE)(LEX_KIND_BLANK_SPACE)(LEX_KIND_SEMICOLON)(LEX_KIND_PAREN_OPEN)(LEX_KIND_PAREN_CLOSE)(LEX_KIND_SQUARE_OPEN)(LEX_KIND_SQUARE_CLOSE)(LEX_KIND_CURLY_OPEN)(LEX_KIND_CURLY_CLOSE)(LEX_KIND_COMMA)(LEX_KIND_DOT)(LEX_KIND_ASSIGN)(LEX_KIND_ASSIGN_INFER)(LEX_KIND_COLON)
    /** Reserved keywords */
    (LEX_KIND_IF)(LEX_KIND_FN)
    /** Variable values */
    (LEX_KIND_SYMBOL)

    /** Literals */
    (LEX_KIND_LITERAL_STRING)(LEX_KIND_LITERAL_INT)(LEX_KIND_LITERAL_FLOAT)

);

typedef struct Lexeme {
    LexemeKind kind;
    Ck_LenStr value;
} Lexeme;

void lexeme_printf(Lexeme* lexeme) {
    if (lexeme->kind == LEX_KIND_EOF) {
        printf("<Lexeme EOF>");
        return;
    }

    printf("<Lexeme %s ", lexeme_kind_to_str(lexeme->kind));

    if (lexeme->value._length <= 10) {
        printf("'%.*s'>", (int)lexeme->value._length, lexeme->value._str);
    } else {
        printf("'%.10s...'>", lexeme->value._str);
    }
}

#endif /** __ZOST_CORE_ANALYSIS_C__ */