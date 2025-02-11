#ifndef __ZORST_PHASE_LEXER_STATE_C__
#define __ZORST_PHASE_LEXER_STATE_C__

#include "core.c"

LexerState lexer_state_start(Lexer* lexer);
LexerState lexer_state_literal_int(Lexer* lexer);
LexerState lexer_state_literal_float(Lexer* lexer);
LexerState lexer_state_arithmetic_operator(Lexer* lexer);
LexerState lexer_state_paren_opened(Lexer* lexer);
LexerState lexer_state_comment_inline(Lexer* lexer);
LexerState lexer_state_comment_block(Lexer* lexer);
// LexerState lexer_state_alphabetic(Lexer* lexer);
// LexerState lexer_state_inside_string(Lexer* lexer);

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

    /**
     * Comments check if the char pattern matches the opening for a comment
     * and if it does then skips the char.
     */
    if (c == '/' && scanner->peek(scanner) == '/') {
        scanner->next(scanner);
        return _lextate(lexer_state_comment_inline);
    }
    if (c == '/' && scanner->peek(scanner) == '*') {
        scanner->next(scanner);
        return _lextate(lexer_state_comment_block);
    }

    if (c == '/' || c == '*' || c == '%') {
        return _lextate(lexer_state_arithmetic_operator);
    }
    if (c == '+' || c == '-' || (c >= '0' && c <= '9')) {
        return _lextate(lexer_state_literal_int);
    }
    if (c == '.') {
        return _lextate(lexer_state_literal_float);
    }
    if (c == '(') {
        return _lextate(lexer_state_paren_opened);
    }
    if (c == ')') {
        lexer_consume(lexer, c);
        lexer_emit(lexer, LEX_KIND_PAREN_CLOSE);
        return _lextate(lexer_state_start);
    }

    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        // return _lextate(lexer_state_alphabetic);
    }

    lexer_emit(lexer, LEX_KIND_ERROR);

    return _lextate(NULL);
}

LexerState lexer_state_arithmetic_operator(Lexer* lexer) {
    char previous_char = lexer->_scanner->_current;

    lexer_consume(lexer, previous_char);

    Scanner* scanner = lexer->_scanner;
    char c = scanner->next(scanner);

    if (previous_char == '*' && c == '*') {
        lexer_consume(lexer, c);
        c = scanner->next(scanner);
    }

    while (c == ' ' || c == '\t' || c == '\n') {
        c = scanner->next(scanner);
    }

    if (c == '+' || c == '-' || (c >= '0' && c <= '9')) {
        lexer_emit(lexer, LEX_KIND_ARITHMETIC_OPERATOR);
        return _lextate(lexer_state_literal_int);
    }
    if (c == '.') {
        lexer_emit(lexer, LEX_KIND_ARITHMETIC_OPERATOR);
        return _lextate(lexer_state_literal_float);
    }

    lexer_emit(lexer, LEX_KIND_ERROR);

    return _lextate(NULL);
}

LexerState lexer_state_literal_int(Lexer* lexer) {
    char previous_char = lexer->_scanner->_current;

    lexer_consume(lexer, previous_char);

    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while (
        (c = scanner->next(scanner)) &&
        (c == ' ' || c == '\t' || c == '+' || c == '-')

    ) {
        if ((c == '+' || c == '-') && previous_char >= '0' && previous_char <= '9') {
            lexer_emit(lexer, LEX_KIND_LITERAL_NUMBER);
            return _lextate(lexer_state_arithmetic_operator);
        }

        if (
            (c == '+' || c == '-') &&
            (previous_char == '+' || previous_char == '-') &&
            lexer->_last_state.run != lexer_state_literal_int &&
            lexer->_last_state.run != lexer_state_start

        ) {
            lexer_emit(lexer, LEX_KIND_ARITHMETIC_OPERATOR);
            return _lextate(lexer_state_literal_int);
        } else if ((c == '+' || c == '-')) {
            lexer_consume(lexer, c);
        }
    }

    while (c >= '0' && c <= '9') {
        lexer_consume(lexer, c);
        c = scanner->next(scanner);
    }

    if (c == '/' || c == '*' || c == '%') {
        lexer_emit(lexer, LEX_KIND_LITERAL_NUMBER);
        return _lextate(lexer_state_arithmetic_operator);
    }
    if (c == '+' || c == '-' || (c >= '0' && c <= '9')) {
        lexer_emit(lexer, LEX_KIND_LITERAL_NUMBER);
        return _lextate(lexer_state_literal_int);
    }

    if (c == ' ' || c == '\n' || c == '\t' || c == ';') {
        lexer_emit(lexer, LEX_KIND_LITERAL_NUMBER);
        return _lextate(lexer_state_start);
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

LexerState lexer_state_literal_float(Lexer* lexer) {
    lexer_consume(lexer, lexer->_scanner->_current);

    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while ((c = scanner->next(scanner)) && c >= '0' && c <= '9') {
        lexer_consume(lexer, c);
    }

    if (c == '+' || c == '-' || c == '/' || c == '*' || c == '%') {
        lexer_emit(lexer, LEX_KIND_LITERAL_NUMBER);
        return _lextate(lexer_state_arithmetic_operator);
    }
    if (c >= '0' && c <= '9') {
        lexer_emit(lexer, LEX_KIND_LITERAL_NUMBER);
        return _lextate(lexer_state_start);
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

LexerState lexer_state_paren_opened(Lexer* lexer) {
    lexer_consume(lexer, lexer->_scanner->_current);

    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while ((c = scanner->next(scanner)) && (c == ' ' || c == '\t' || c <= '\n'));

    if (c == ';') {
        lexer_emit(lexer, LEX_KIND_ERROR);
        _lextate(NULL);
    }

    lexer_emit(lexer, LEX_KIND_PAREN_OPEN);

    return _lextate(lexer_state_start);
}

LexerState lexer_state_comment_inline(Lexer* lexer) {
    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while ((c = scanner->next(scanner)) != '\n');

    return _lextate(lexer_state_start);
}

LexerState lexer_state_comment_block(Lexer* lexer) {
    Scanner* scanner = lexer->_scanner;
    char c = 0;

    while ((c = scanner->next(scanner)) != '*' || scanner->peek(scanner) != '/');

    scanner->next(scanner);

    return _lextate(lexer_state_start);
}

#endif /* __ZORST_PHASE_LEXER_STATE_C__ */
