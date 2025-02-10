#ifndef __ZORST_PHASE_LEXER_STATE_C__
#define __ZORST_PHASE_LEXER_STATE_C__

#include "core.c"

typedef struct LexerState {
    struct LexerState (*run)(Lexer*);
} LexerState;
typedef LexerState (*LexerStateRunFn)(Lexer*);

LexerState _lextate(LexerStateRunFn caller) {
    LexerState state = {.run = caller};
    return state;
}

LexerState lexer_state_start(Lexer* lexer);
LexerState lexer_state_sign(Lexer* lexer);
LexerState lexer_state_literal_int(Lexer* lexer);
LexerState lexer_state_literal_float(Lexer* lexer);
// LexerState lexer_state_operator(Lexer* lexer);
// LexerState lexer_state_alphabetic(Lexer* lexer);
// LexerState lexer_state_inside_string(Lexer* lexer);
// LexerState lexer_state_inside_bracket(Lexer* lexer);

LexerState lexer_state_start(Lexer* lexer) {
    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while (
        (c = scanner->next(scanner)) &&
        (c == ' ' || c == '\n' || c == '\t' || c == ';')

    );

    if (c == EOF) {
        lexer_emit(lexer, LEX_KIND_EOF);

        return _lextate(NULL);
    }

    if (c == '+' || c == '-') {
        return _lextate(lexer_state_sign);
    }
    if (c == '/' || c == '*' || c == '%') {
        lexer_consume(lexer, c);
        lexer_emit(lexer, LEX_KIND_ARITHMETIC_OPERATOR);
        return _lextate(lexer_state_start);
    }
    if (c >= '0' && c <= '9') {
        return _lextate(lexer_state_literal_int);
    }
    if (c == '.') {
        return _lextate(lexer_state_literal_float);
    }

    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        // return _lextate(lexer_state_alphabetic);
    }

    lexer_emit(lexer, LEX_KIND_ERROR);

    return _lextate(NULL);
}

LexerState lexer_state_sign(Lexer* lexer) {
    lexer_consume(lexer, lexer->_scanner->_current);

    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while (
        (c = scanner->next(scanner)) &&
        (c == ' ' || c == '\t' || c == '+' || c == '-')

    ) {
        if (c == '+' || c == '-') {
            lexer_consume(lexer, c);
        }
    }

    if (c >= '0' && c <= '9') {
        return _lextate(lexer_state_literal_int);
    }
    if (c == '.') {
        return _lextate(lexer_state_literal_float);
    }

    if (c == EOF) {
        lexer_emit(lexer, LEX_KIND_EOF);
        return _lextate(NULL);
    }

    lexer_emit(lexer, LEX_KIND_ERROR);

    return _lextate(NULL);
}

LexerState lexer_state_literal_int(Lexer* lexer) {
    lexer_consume(lexer, lexer->_scanner->_current);

    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while ((c = scanner->next(scanner)) && c >= '0' && c <= '9') {
        lexer_consume(lexer, c);
    }

    if (c == '.') {
        return _lextate(lexer_state_literal_float);
    }

    if (c == ' ' || c == '\n' || c == '\t' || c == ';') {
        lexer_emit(lexer, LEX_KIND_LITERAL_NUMBER);
        return _lextate(lexer_state_start);
    }

    if (c == EOF) {
        lexer_emit(lexer, LEX_KIND_EOF);
        return _lextate(NULL);
    }

    lexer_emit(lexer, LEX_KIND_ERROR);

    return _lextate(NULL);
}

LexerState lexer_state_literal_float(Lexer* lexer) {
    lexer_consume(lexer, lexer->_scanner->_current);

    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while ((c = scanner->next(scanner)) && c >= '0' && c <= '9') {
        lexer_consume(lexer, c);
    }

    if (c == ' ' || c == '\n' || c == '\t' || c == ';') {
        lexer_emit(lexer, LEX_KIND_LITERAL_NUMBER);
        return _lextate(lexer_state_start);
    }

    if (c == EOF) {
        lexer_emit(lexer, LEX_KIND_EOF);
        return _lextate(NULL);
    }

    lexer_emit(lexer, LEX_KIND_ERROR);

    return _lextate(NULL);
}

#endif /* __ZORST_PHASE_LEXER_STATE_C__ */
