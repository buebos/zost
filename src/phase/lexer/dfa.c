#ifndef __CKASTAL_PHASE_LEXER_DFA_C__
#define __CKASTAL_PHASE_LEXER_DFA_C__

#include "../../core/analysis.c"
#include "../../service/scanner/provider/file.c"

#ifndef LEXER_LEXEME_BUFFER_SIZE
#define LEXER_LEXEME_BUFFER_SIZE 128
#endif

typedef struct LexerDfaTransition LexerDfaTransition;

typedef struct LexerDfaState {
    /**
     * The current lexeme kind of the node; when a node with kind different
     * than incomplete is reached it should emit a lexeme.
     */
    LexemeKind lexeme_kind;

    /**
     * The next states with their corresponding cargos.
     */
    LexerDfaTransition *transitions;

    size_t transitions_len;
} LexerDfaState;

typedef struct LexerDfaTransitionString {
    /**
     * The chars that can lead to the current state.
     */
    char *cargo;
} LexerDfaTransitionString;

typedef struct LexerDfaTransitionFn {
    bool (*run)(char);
} LexerDfaTransitionFn;

typedef enum LexerDfaTransitionType {
    LEX_DFA_TRANSITION_TYPE_STRING = 0,
    LEX_DFA_TRANSITION_TYPE_FN,
} LexerDfaTransitionType;

typedef union LexerDfaTransitionUnion {
    LexerDfaTransitionString transition_str;
    LexerDfaTransitionFn transition_fn;
} LexerDfaTransitionUnion;

typedef struct LexerDfaTransition {
    LexerDfaTransitionType type;
    LexerDfaTransitionUnion value;
    LexerDfaState *state_target;
} LexerDfaTransition;

typedef struct LexerDfa {
    Scanner *scanner;
    Lexeme current;
    LexerDfaState *state_start;
    LexerDfaState *state_current;
} LexerDfa;

LexerDfaTransition lexer_dfa_transition_init(LexerDfaTransitionType type, void *value, LexerDfaState *state_target) {
    LexerDfaTransition transition = {
        .type = type,
        .value = {0},
        .state_target = state_target,
    };

    switch (type) {
        case LEX_DFA_TRANSITION_TYPE_STRING:
            transition.value.transition_str.cargo = (char *)value;
            break;
        case LEX_DFA_TRANSITION_TYPE_FN:
            transition.value.transition_fn.run = (bool (*)(char))value;
            break;
    }

    return transition;
}

LexerDfa lexer_dfa_init(Scanner *scanner, LexerDfaState *state_start) {
    LexerDfa lexer = {
        .scanner = scanner,
        .state_start = state_start,
        .state_current = state_start,
        .current = {
            .kind = LEX_KIND_INCOMPLETE,
            .value = ck_lenstr_init(NULL, 0, 0),
        },
    };

    return lexer;
}

void lexer_dfa_consume(LexerDfa *lexer, char c) {
    if (ck_lenstr_space(&lexer->current.value) < 1) {
        printf("[ERROR]: Attempting to consume another char but buffer has run out of space.");
    }

    ck_lenstr_pushc(&lexer->current.value, c);
}

void lexer_dfa_emit(LexerDfa *lexer, LexemeKind kind) {
    if (kind == LEX_KIND_ERROR) {
        if (lexer->scanner->_current != '\n') {
            lexer_dfa_consume(lexer, lexer->scanner->_current);
        } else {
            lexer_dfa_consume(lexer, '\\');
            lexer_dfa_consume(lexer, 'n');
        }
    }

    lexer->current.kind = kind;
}

Lexeme *lexer_dfa_next(LexerDfa *lexer) {
    lexer->state_current = lexer->state_start;

    lexer->current.kind = LEX_KIND_INCOMPLETE;
    lexer->current.value = ck_lenstr_init(
        ck_memory.alloc(LEXER_LEXEME_BUFFER_SIZE),
        LEXER_LEXEME_BUFFER_SIZE,
        0

    );

    char c = 0;

    while ((c = lexer->scanner->next(lexer->scanner)) != EOF) {
        LexerDfaTransition *transition = lexer->state_current->transitions;
        bool is_state_changed = false;

        for (size_t i = 0; i < lexer->state_current->transitions_len; i++) {
            switch (transition->type) {
                case LEX_DFA_TRANSITION_TYPE_STRING:
                    char *cargo = transition->value.transition_str.cargo;
                    size_t i = 0;

                    while (cargo[i] != 0) {
                        if (cargo[i] == c) {
                            lexer_dfa_consume(lexer, c);
                            lexer->state_current = transition->state_target;
                            is_state_changed = true;
                            break;
                        }
                        i++;
                    }

                    break;
                case LEX_DFA_TRANSITION_TYPE_FN:
                    if (transition->value.transition_fn.run(c)) {
                        lexer_dfa_consume(lexer, c);

                        lexer->state_current = transition->state_target;
                        is_state_changed = true;
                    }

                    break;
            }

            if (is_state_changed) {
                lexer_dfa_emit(lexer, lexer->state_current->lexeme_kind);
                break;
            }

            transition++;
        }

        /**
         * No matching transition has been found for the incoming char with the
         * current state.
         */
        if (!is_state_changed) {
            lexer_dfa_emit(lexer, LEX_KIND_ERROR);
            return &lexer->current;
        }
    }

    return &lexer->current;
}

#endif /* __CKASTAL_PHASE_LEXER_DFA_C__ */
