#ifndef __ZOST_PHASE_LEXER_MAIN_C__
#define __ZOST_PHASE_LEXER_MAIN_C__

#include <stdio.h>

#include "../../../vendor/ckastal/include/init.c"
#include "../../core/analysis.c"
#include "../../core/status.c"

typedef struct Tokenizer {
    FILE* _reader;
    size_t _count;
    char* _cursor;

    size_t _line;
    size_t _column;
} Tokenizer;

Tokenizer tokenizer_init(FILE* reader) {
    Tokenizer tokenizer = {
        ._reader = reader,
        ._count = 0,
        ._cursor = NULL,
    };

    return tokenizer;
}

Token tokenizer_next(Tokenizer* tokenizer) {
    Token tok = {
        .kind = TOKEN_KIND_UNKNOWN,
        .value = ck_lenstr_init(NULL, 0),
    };

    while (TOKEN_KIND_UNKNOWN == tok.kind) {
        char c = fgetc(tokenizer->_reader);

        if (c == EOF) {
            status.type = STATUS_TYPE_ERROR;
            status.title = "Reached end of file before identifying a token.";
            status._source_line = tokenizer->_line;
            status._source_column = tokenizer->_column;
            status._source_cursor = tokenizer->_cursor;

            return tok;
        }

        switch (c) {
        }
    }

    return tok;
}

#endif /* __ZOST_PHASE_LEXER_MAIN_C__ */
