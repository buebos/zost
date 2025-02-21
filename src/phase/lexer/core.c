#ifndef __ZOST_PHASE_LEXER_MAIN_C__
#define __ZOST_PHASE_LEXER_MAIN_C__

#include "../../../vendor/ckastal/include/init.c"
#include "../../core/analysis.c"
#include "../../service/scanner/provider/file.c"

const short LEXER_LEXEME_BUFFER_SIZE = 1024;

typedef struct Lexer Lexer;

typedef struct LexerState {
    struct LexerState (*run)(Lexer*);
} LexerState;
typedef LexerState (*LexerStateRunFn)(Lexer*);

typedef struct Lexer {
    Scanner* _scanner;
    Lexeme _current;
    LexerState _last_state;
} Lexer;

LexerState _lextate(LexerStateRunFn caller) {
    LexerState state = {.run = caller};
    return state;
}

Lexer lexer_init(Scanner* scanner) {
    char* buffer = ck_memory.alloc(sizeof(char) * LEXER_LEXEME_BUFFER_SIZE);
    Lexer lexer = {
        ._scanner = scanner,
        ._current = (Lexeme){
            .kind = LEX_KIND_INCOMPLETE,
            .value = ck_lenstr_init(buffer, LEXER_LEXEME_BUFFER_SIZE, 0),
        },
        ._last_state = {.run = NULL},
    };

    return lexer;
}

void lexer_consume(Lexer* lexer, char c) {
    if (ck_lenstr_space(&lexer->_current.value) < 1) {
        printf("[ERROR]: Attempting to consume another char but buffer has run out of space.");
    }

    ck_lenstr_pushc(&lexer->_current.value, c);
}

void lexer_emit(Lexer* lexer, LexemeKind kind) {
    if (kind == LEX_KIND_ERROR) {
        if (lexer->_scanner->_current != '\n') {
            lexer_consume(lexer, lexer->_scanner->_current);
        } else {
            lexer_consume(lexer, '\\');
            lexer_consume(lexer, 'n');
        }
    }

    lexer->_current.kind = kind;
}

Lexeme lexer_last(Lexer* lexer) {
    return lexer->_current;
}

#include "state.c"

Lexeme* lexer_next(Lexer* lexer) {
    if (lexer_last(lexer).kind == LEX_KIND_EOF) {
        return NULL;
    }

    LexerState state =
        lexer->_last_state.run != NULL
            ? lexer->_last_state
            : _lextate(lexer_state_start);

    lexer->_current = (Lexeme){
        .kind = LEX_KIND_INCOMPLETE,
        .value = lexer->_current.value,
    };

    lexer->_current.value._length = 0;

    while (NULL != state.run && LEX_KIND_INCOMPLETE == lexer_last(lexer).kind) {
        state = state.run(lexer);
    }

    lexer->_last_state = state;

    return &lexer->_current;
}

#endif /* __ZOST_PHASE_LEXER_MAIN_C__ */
