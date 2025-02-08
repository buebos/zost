#ifndef __ZOST_CORE_ANALYSIS_C__
#define __ZOST_CORE_ANALYSIS_C__

#include <stdio.h>

#include "../../vendor/ckastal/include/prefixed_length_string.c"
#include "../../vendor/ckastal/include/syntax_utils.h"

enum_str_setup(TokenKind,
               "TOKEN_KIND",
               TOKEN_KIND_STRINGS,
               token_kind_to_str,
               /** This will start as set to zero in the enum */
               TOKEN_KIND_ERROR,
               /** Other error or aside of language token types */
               (TOKEN_KIND_UNKNOWN)(TOKEN_KIND_EOF)

               /* Comments */
               (TOKEN_KIND_COMMENT_LINE_START)(TOKEN_KIND_COMMENT_BLOCK_START)
               /** Arithmetical operators */
               (TOKEN_KIND_PLUS)(TOKEN_KIND_MINUS)(TOKEN_KIND_MULTIPLY)(TOKEN_KIND_DIVIDE)(TOKEN_KIND_POWER)
               /** Logical operators */
               (TOKEN_KIND_EQUALS)(TOKEN_KIND_GT)(TOKEN_KIND_GTE)(TOKEN_KIND_LT)(TOKEN_KIND_LTE)(TOKEN_KIND_OR)(TOKEN_KIND_AND)
               /** Delimiters  */
               (TOKEN_KIND_NEW_LINE)(TOKEN_KIND_BLANK_SPACE)(TOKEN_KIND_SEMICOLON)(TOKEN_KIND_PAREN_OPEN)(TOKEN_KIND_PAREN_CLOSE)(TOKEN_KIND_SQUARE_OPEN)(TOKEN_KIND_SQUARE_CLOSE)(TOKEN_KIND_CURLY_OPEN)(TOKEN_KIND_CURLY_CLOSE)(TOKEN_KIND_COMMA)(TOKEN_KIND_DOT)(TOKEN_KIND_ASSIGN)(TOKEN_KIND_ASSIGN_INFER)(TOKEN_KIND_COLON)
               /** Reserved keywords */
               (TOKEN_KIND_IF)(TOKEN_KIND_FN)
               /** Variable values */
               (TOKEN_KIND_SYMBOL)(TOKEN_KIND_LITERAL_STRING)(TOKEN_KIND_LITERAL_NUMBER));

typedef struct Token {
    TokenKind kind;
    Ck_LenStr value;
} Token;

void token_printf(Token* tok) {
    if (tok->kind == TOKEN_KIND_EOF) {
        printf("<Token EOF>");
        return;
    }
    if (tok->kind <= TOKEN_KIND_FN) {
        printf("<Token %s '%s'>", token_kind_to_str(tok->kind), tok->value._str);
        return;
    }

    printf("<Token %s '%.10s'>", token_kind_to_str(tok->kind), tok->value._str);
}

#endif /** __ZOST_CORE_ANALYSIS_C__ */